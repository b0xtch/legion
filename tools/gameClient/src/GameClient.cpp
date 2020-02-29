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


void initializeMenuPages( MenuManager &menuManager, bool &done,
                            networking::Client &client, ChatWindow *chatWindow,
                            auto &onTextEntry );

// Make json message for server
std::string makeServerMessage(string input);

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
    ChatWindow *chatWindow = nullptr;

    MenuManager menuManager;

    initializeMenuPages( menuManager, done, client, chatWindow, onTextEntry );


    // Start menu

    menuManager.initializeStartingPage();

    while (!done && !client.isDisconnected()) {

        try {
            client.update();
        } catch (std::exception& e) {
            printf("Exception from Client update.");
            done = true;
        }

        if (currentMode == mainMenu) {

            menuManager.update();

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

    if ( chatWindow != nullptr ) {
        delete chatWindow;
    }

    menuManager.cleanup();

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


void initializeMenuPages( MenuManager &menuManager, bool &done,
                            networking::Client &client, ChatWindow *chatWindow,
                            auto &onTextEntry ) {

    // Main menu
    MenuPage::NameList mainMenuGields = {};

    MenuPage::NameList mainMenuItems = { "Join lobby", "Create lobby", "Exit" };

    // Main menu item functions
    auto moveToJoinLobbyPage = [&menuManager] () {
        MenuPage::MenuName nextPage = "Join lobby";
        menuManager.switchPage( nextPage );
    };

    auto moveToCreateLobbyPage = [&client, &menuManager] () {

        // Get the list of games

        std::string commadType =  "!requestGames ";
        std::string serverMessage = makeServerMessage( commadType );
        client.send( serverMessage );

        auto response = client.receive();
        //while ( response.empty() ) {
        //    response = client.receive();
        //}

        MenuPage::NameList gamesList = {"Game 1", "Game 2"};
            /* TEMP create list from response */

        // Define the function that is called when a game is chosen

        auto chooseGame = [&client, &gamesList, &menuManager] () {

            //std::string gameChoice = gamesList[menuManager.getSelectedIndex()];
            //client.send( gameChoice );

        };

        // Create the "Creat lobby" menu page with the given games

        MenuPage::NameList createLobbyGields = {};

        MenuPage::NameList createLobbyItems = {};

        for ( auto gameName : gamesList ) {
            createLobbyItems.push_back( gameName );
        }
        MenuPage::ItemName backString = "back";
        createLobbyItems.push_back( backString );

        MenuPage::FunctionList createLobbyItemResults = {};

        for ( int i = 0; i < gamesList.size(); i++ ) {
            createLobbyItemResults.push_back( chooseGame );
        }

        auto moveBackToMainMenuPage = [&menuManager] () {
            MenuPage::MenuName nextPage = "Main menu";
            menuManager.switchPage( nextPage );
        };
        createLobbyItemResults.push_back( moveBackToMainMenuPage );

        MenuPage::MenuName createLobbyName = "Create lobby";
        MenuPage *createLobbyPage = new MenuPage( createLobbyName,
                                                    createLobbyGields,
                                                    createLobbyItems,
                                                    createLobbyItemResults );
        menuManager.addPage( createLobbyPage );

        // The games are all loaded and the page is setup, so switch to the page

        MenuPage::MenuName nextPage = "Create lobby";
        menuManager.switchPage( nextPage );
    };

    auto exitProgram = [&done] () {
        done = true;
    };

    MenuPage::FunctionList mainMenuItemResults = { moveToJoinLobbyPage,
                                                      moveToCreateLobbyPage,
                                                      exitProgram };

    MenuPage::MenuName mainMenuName = "Main menu";
    MenuPage *mainMenuPage = new MenuPage( mainMenuName,
                                             mainMenuGields,
                                             mainMenuItems,
                                             mainMenuItemResults );
    menuManager.addPage( mainMenuPage );



    // Join lobby menu
    const MenuPage::NameList joinLobbyGields = {"Lobby code:"};

    const MenuPage::NameList joinLobbyItems = {"Join", "Back"};

    // Join lobby item functions

    auto moveBackToMainMenuPage = [&menuManager] () {
        MenuPage::MenuName nextPage = "Main menu";
        menuManager.switchPage( nextPage );
    };

    auto joinLobby = [&done, &client, &chatWindow, &onTextEntry, &menuManager] () {
        std::string commadType =  "!joinsession ";

        MenuPage *joinPage = menuManager.getCurrentPage();
        MenuPage::FieldList *connectGields = joinPage->getFieldList();

        const int lobbyCodeInputIndex = 1;
        const char *lobbyCode =
            field_buffer( connectGields->at( lobbyCodeInputIndex ), 0 );
        std::string lobbyCodeString(lobbyCode);

        std::string command = commadType + lobbyCodeString;
        std::string serverMessage = makeServerMessage( command );
        client.send( serverMessage );

        currentMode = chatMenu;
        menuManager.cleanup();
        chatWindow = new ChatWindow(onTextEntry);
    };

    const MenuPage::FunctionList joinLobbyItemResults
        = {joinLobby, moveBackToMainMenuPage};

    MenuPage::MenuName joinLobbyName = "Join lobby";
    MenuPage *joinLobbyPage = new MenuPage( joinLobbyName,
                                              joinLobbyGields,
                                              joinLobbyItems,
                                              joinLobbyItemResults );
    menuManager.addPage( joinLobbyPage );

    menuManager.setCurrentPage( mainMenuPage );
}
