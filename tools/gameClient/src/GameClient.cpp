#include <iostream>
#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include "json.hpp"

#include "MenuPage.h"
#include "MenuManager.h"
#include "ChatWindow.h"
#include "Client.h"

using namespace std;
using json = nlohmann::json;

// Basic menu structure referenced from: 
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html

// Basic menu structure referenced from: 
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/forms.html

// Idea on combining menus and forms on one page referenced from:
//    https://alan-mushi.github.io/2015/05/26/simple-ncurses-popup-in-C.html


void initialize_menu_pages( MenuManager &menu_manager, bool &done,
                            networking::Client &client, ChatWindow *chatWindow, 
                            auto &onTextEntry );

// Make json message for server
string makeServerMessage(string input);

// Controls which window is active
enum menuMode { mainMenu, chatMenu };
menuMode currentMode;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("Incorrect Arguments");
        return 1;
    }

    bool done = false;
    currentMode = mainMenu;
    networking::Client client{argv[1], argv[2]};

    // Start Chat Window
    auto onTextEntry = [&done, &client] (std::string text) {
    if ("!exit" == text || "!quit" == text) {
            done = true;
        } else {
            client.send( makeServerMessage(text) );
        }
    };
    ChatWindow *chatWindow;

    MenuManager menu_manager;
    
    initialize_menu_pages( menu_manager, done, client, chatWindow, onTextEntry );


    // Start menu

    menu_manager.initialize_starting_page();

    while (!done && !client.isDisconnected()) {

        try {
            client.update();
        } catch (std::exception& e) {
            printf("Exception from Client update.");
            done = true;
        }

        if (currentMode == mainMenu) {

            menu_manager.main_menu_driver();

        } else if (currentMode == chatMenu) {

            auto response = client.receive();
            if (!response.empty()) {
                chatWindow->displayText(response);
            }
            chatWindow->update();

        } else {

            printf("Incorrect Mode.");
            done = true;

        }

    }

    delete chatWindow;

    menu_manager.cleanup();

    return 0;
}

/** 
Takes a string input and converts it into a json object consisting of 2 fields.
The first field is the command to be given to the server. The second field is any extra arguments for the command.

If the first word of the input matches a possible command, it is set as the value for the command field.
Then the rest of the input is put in the data field.

If the first word does not match a possible command, the input is assumed to be a chat message.
The command is set to !chat, and the entire input is put in the data field.
**/
string makeServerMessage(string input) {
    
    vector<string> possibleCommands;
    possibleCommands.push_back("!createsession");
    possibleCommands.push_back("!joinsession");
    possibleCommands.push_back("!leavesession");
    possibleCommands.push_back("!gameinput");
    possibleCommands.push_back("!whisper");
    possibleCommands.push_back("!requestGames");

    string command = "{ \"command\": \"";
    
    size_t endOfCommand = input.find(" ");
    string firstWord = input.substr(0,endOfCommand);
    if ( std::find(possibleCommands.begin(), possibleCommands.end(), firstWord) != possibleCommands.end() ) {
        command += firstWord;
    } else {
        command += "!chat";
        endOfCommand = 0;
    }

    string data = "\", \"data\": \"";
    data += input.substr(endOfCommand, string::npos);

    auto message = json::parse( command+data+"\" }" );

    return message.dump();

}


void initialize_menu_pages( MenuManager &menu_manager, bool &done,
                            networking::Client &client, ChatWindow *chatWindow, 
                            auto &onTextEntry ) {

    // Main menu
    MenuPage::NameList main_menu_fields = {};

    MenuPage::NameList main_menu_items = { "Join lobby", "Create lobby", "Exit" };

    // Main menu item functions
    auto moveToJoinLobbyPage = [&menu_manager] () {
        std::string next_page = "Join lobby";
        menu_manager.switch_page( next_page );
    };

    auto moveToCreateLobbyPage = [&client, &menu_manager] () {

        // Get the list of games

        std::string commad_type =  "!requestGames ";
        std::string serverMessage = makeServerMessage( commad_type );
        client.send( serverMessage );

        auto response = client.receive();
        //while ( response.empty() ) {
        //    response = client.receive();
        //}

        MenuPage::NameList games_list = {"Game 1", "Game 2"};
            /* TEMP create list from response */

        // Define the function that is called when a game is chosen

        auto chooseGame = [&client, &games_list, &menu_manager] () {

            //std::string game_choice = games_list[menu_manager.get_selected_index()];
            //client.send( game_choice );

        };

        // Create the "Creat lobby" menu page with the given games

        MenuPage::NameList create_lobby_fields = {};

        MenuPage::NameList create_lobby_items = {};

        for ( const char *game_name : games_list ) {
            create_lobby_items.push_back( game_name );
        }
        const char *back_string = "back";
        create_lobby_items.push_back( back_string );

        MenuPage::FunctionList create_lobby_item_results = {}; 

        for ( int i = 0; i < games_list.size(); i++ ) {
            create_lobby_item_results.push_back( chooseGame );
        }
        
        auto moveBackToMainMenuPage = [&menu_manager] () {
            std::string next_page = "Main menu";
            menu_manager.switch_page( next_page );
        };
        create_lobby_item_results.push_back( moveBackToMainMenuPage );     

        std::string create_lobby_name = "Create lobby";
        MenuPage *create_lobby_page = new MenuPage( create_lobby_name,
                                                    create_lobby_fields, 
                                                    create_lobby_items, 
                                                    create_lobby_item_results );  
        menu_manager.add_page( create_lobby_page ); 

        // The games are all loaded and the page is setup, so switch to the page

        std::string next_page = "Create lobby";
        menu_manager.switch_page( next_page );
    };

    auto exitProgram = [&done] () {
        done = true;
    };
    
    MenuPage::FunctionList main_menu_item_results = { moveToJoinLobbyPage,
                                                      moveToCreateLobbyPage,
                                                      exitProgram };

    std::string main_menu_name = "Main menu";
    MenuPage *main_menu_page = new MenuPage( main_menu_name,
                                             main_menu_fields, 
                                             main_menu_items, 
                                             main_menu_item_results );
    menu_manager.add_page( main_menu_page );



    // Join lobby menu
    const MenuPage::NameList join_lobby_fields = {"Lobby code:"};

    const MenuPage::NameList join_lobby_items = {"Join", "Back"};
        
    // Join lobby item functions

    auto moveBackToMainMenuPage = [&menu_manager] () {
        std::string next_page = "Main menu";
        menu_manager.switch_page( next_page );
    };

    auto joinLobby = [&done, &client, &chatWindow, &onTextEntry, &menu_manager] () {
        std::string commad_type =  "!joinsession ";

        MenuPage *join_page = menu_manager.get_current_page();
        MenuPage::FieldList *connect_fields = join_page->get_field_list();

        const int lobby_code_input_index = 1;
        const char *lobby_code = 
            field_buffer( connect_fields->at( lobby_code_input_index ), 0 );
        std::string lobby_code_string(lobby_code);

        std::string command = commad_type + lobby_code_string;
        std::string serverMessage = makeServerMessage( command );
        client.send( serverMessage );

        currentMode = chatMenu;
        menu_manager.cleanup();
        chatWindow = new ChatWindow(onTextEntry);
    };

    const MenuPage::FunctionList join_lobby_item_results
        = {joinLobby, moveBackToMainMenuPage};

    string join_lobby_name = "Join lobby";
    MenuPage *join_lobby_page = new MenuPage( join_lobby_name,
                                              join_lobby_fields,
                                              join_lobby_items, 
                                              join_lobby_item_results );
    menu_manager.add_page( join_lobby_page );

    menu_manager.set_current_page( main_menu_page );
}
