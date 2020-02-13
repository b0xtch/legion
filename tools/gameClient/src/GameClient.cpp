#include <iostream>
#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include "json.hpp"

#include "MenuPage.h"
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

    MenuManager::initialize_menu_manager();

    // Initialize menu pages

    // Main menu
    MenuPage::NameList main_menu_fields = {};

    MenuPage::NameList main_menu_items 
        = {"Join lobby", "Create lobby", "Exit"};

    // Main menu item functions
    auto moveToJoinLobbyPage = [] () {
        std::string next_page = "Join lobby";
        MenuManager::switch_page( next_page );
    };

    auto moveToCreateLobbyPage = [] () {
        std::string next_page = "Join lobby";
        MenuManager::switch_page( next_page );
    };

    auto exitProgram = [&done] () {
        done = true;
    };

    const MenuPage::FunctionList main_menu_item_results
        = {moveToJoinLobbyPage, moveToCreateLobbyPage, exitProgram};

    string main_menu_name = "Main menu";
    MenuPage *main_menu_page = new MenuPage( main_menu_name,
                                            main_menu_fields, 
                                            main_menu_items, 
                                            main_menu_item_results );
    MenuManager::add_menu_page( main_menu_name, main_menu_page );


    // Join lobby menu
    MenuPage::NameList join_lobby_fields
        = {"Lobby code:"};

    MenuPage::NameList join_lobby_items
        = {"Join", "Back"};
        
    // Join lobby item functions
    auto joinLobby = [&done, &client, &chatWindow, &onTextEntry] () {
        std::string commad_type =  "!joinsession ";

        MenuPage *join_page = MenuManager::get_current_page();
        vector<FIELD *> connect_fields = join_page->get_field_list();

        const int lobby_code_input_index = 1;
        const char *lobby_code = 
            field_buffer( connect_fields[lobby_code_input_index], 0 );
        std::string lobby_code_string(lobby_code);

        std::string command = commad_type + lobby_code_string;
        std::string serverMessage = makeServerMessage( command );
        client.send( serverMessage );

        currentMode = chatMenu;
        MenuManager::cleanup();
        chatWindow = new ChatWindow(onTextEntry);
    };

    auto moveBackToMainMenuPage = [] () {
        std::string next_page = "Main menu";
        MenuManager::switch_page( next_page );
    };

    const MenuPage::FunctionList join_lobby_item_results
        = {joinLobby, moveBackToMainMenuPage};

    string join_lobby_name = "Join lobby";
    MenuPage *join_lobby_page = new MenuPage( join_lobby_name,
                                             join_lobby_fields,
                                             join_lobby_items, 
                                             join_lobby_item_results );
    MenuManager::add_menu_page( join_lobby_name, join_lobby_page );


    // Create lobby menu
    // TODO: Make the menu get the playable games, and then move to the game lobby
        // with the generated lobby code displayed
        // (insert game names to start of "create_lobby_items")
    MenuPage::NameList create_lobby_fields = {};
    MenuPage::NameList create_lobby_items
        = {"Back"};

    const MenuPage::FunctionList create_lobby_item_results
        = {moveBackToMainMenuPage, moveBackToMainMenuPage, 
           moveBackToMainMenuPage, moveBackToMainMenuPage
           }; 

    string create_lobby_name = "Create lobby";
    MenuPage *create_lobby_page = new MenuPage( create_lobby_name,
                                               create_lobby_fields, 
                                               create_lobby_items, 
                                               create_lobby_item_results );  
    MenuManager::add_menu_page( create_lobby_name, create_lobby_page ); 

    MenuManager::set_current_page( main_menu_page );

    // Start menu

    MenuManager::initialize_starting_page();

    while (!done && !client.isDisconnected()) {

        try {
            client.update();
        } catch (std::exception& e) {
            printf("Exception from Client update.");
            done = true;
        }

        if (currentMode == mainMenu) {

            MenuManager::get_current_page()->change_selected_option_on_input();
            MenuManager::main_menu_driver();

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

    MenuManager::cleanup();

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
