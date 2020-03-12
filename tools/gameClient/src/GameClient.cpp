#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include <sstream>
#include <memory>
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
                            networking::Client &client );

// Make json message for server
std::string makeServerMessage(const std::string& input);
// Parse json message from server
std::string processServerMessage(const std::string& response);

int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("Incorrect Arguments");
        return 1;
    }

    bool done = false;
    networking::Client client{argv[1], argv[2]};

    // Start Chat Window
    auto onTextEntry = [&done, &client] (std::string text) {
    if ("!exit" == text || "!quit" == text) {
            done = true;
        } else {
            client.send( makeServerMessage(text) );
        }
    };

    ChatWindow chatWindow( onTextEntry );

    MenuManager menuManager( &chatWindow );

    initializeMenuPages( menuManager, done, client );

    // Start menu

    menuManager.initializeStartingPage();
    while (!done && !client.isDisconnected()) {

        try {
            client.update();
        } catch (std::exception& e) {
            printf("Exception from Client update.");
            done = true;
        }

        auto response = client.receive();
        if (!response.empty()) {
            menuManager.displayChatText(processServerMessage(response));
        }

        menuManager.update();

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
std::string makeServerMessage(const std::string& input) {

    vector<std::string> possibleCommands;
    possibleCommands.push_back("!createsession");
    possibleCommands.push_back("!joinsession");
    possibleCommands.push_back("!leavesession");
    possibleCommands.push_back("!gameinput");
    possibleCommands.push_back("!whisper");
    possibleCommands.push_back("!requestgames");

    std::stringstream commandStream;
    commandStream << "{ \"command\": \""; // Start the json object and declare the command field

    size_t endOfCommand = input.find(" ");
    std::string firstWord = input.substr(0,endOfCommand);
    if ( std::find(possibleCommands.begin(), possibleCommands.end(), firstWord) != possibleCommands.end() ) {
        commandStream << firstWord;
    } else {
        commandStream << "!chat";
        endOfCommand = 0;
    }

    commandStream << "\", \"data\": \""; // Declare the data field
    commandStream << input.substr(endOfCommand, string::npos); // Get the data value
    commandStream << "\" }"; // End of the json object

    json message = json::parse( commandStream.str() );

    return message.dump();

}

/**
Takes the string response from the server and converts it into json.
Then gets the command and data from the json and performs action depending on their values.
Returns a string that is to be displayed to the client user.
**/
std::string processServerMessage(const std::string& response) {

    std::stringstream responseData;
    json serverJson = json::parse(response);

    std::string command = serverJson.at("command");
    std::string data = serverJson.at("data");

    if (command == "!chat") {
        responseData << data;
    } else if (command == "!createsession") {
        if (data == "fail") {
            responseData << "There was an error creating the session.";
        } else {
            responseData << "The session was successfully created.";
        }
    } else if (command == "!joinsession") {
        if (data == "fail") {
            responseData << "There was an error joining the session.";
        } else {
            responseData << "You have joined the session: " << data;
        }
    } else if (command == "!leavesession") {
        responseData << "You have left the session: " << data;
    } else if (command == "!gameinput") {
        // WIP
        // The game output should be displayed in a separate log than the chat
        // To do: function that displays game data
        responseData << data;
    } else if (command == "!requestgames") {
        responseData << "Choose a game: " << data;
    }

    return responseData.str();

}

void initializeMenuPages( MenuManager &menuManager, bool &done,
                            networking::Client &client ) {

    // Main menu
    MenuPage::NameList mainMenuFields = {};

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

        MenuPage::NameList createLobbyFields = {};

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
        std::shared_ptr<MenuPage> createLobbyPage = std::make_shared<MenuPage>( 
                                                    createLobbyName,
                                                    createLobbyFields,
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
    std::shared_ptr<MenuPage> mainMenuPage = std::make_shared<MenuPage>( 
                                             mainMenuName,
                                             mainMenuFields,
                                             mainMenuItems,
                                             mainMenuItemResults );
    menuManager.addPage( mainMenuPage );



    // Join lobby menu
    const MenuPage::NameList joinLobbyFields = {"Lobby code:"};

    const MenuPage::NameList joinLobbyItems = {"Join", "Back"};

    // Join lobby item functions

    auto moveBackToMainMenuPage = [&menuManager] () {
        MenuPage::MenuName nextPage = "Main menu";
        menuManager.switchPage( nextPage );
    };

    auto joinLobby = [&done, &client, &menuManager] () {
        std::string commadType =  "!joinsession ";

        std::shared_ptr<MenuPage> joinPage = menuManager.getCurrentPage();
        MenuPage::FieldList *connectFields = joinPage->getFieldList();

        const int lobbyCodeInputIndex = 1;
        const char *lobbyCode =
            field_buffer( connectFields->at( lobbyCodeInputIndex ), 0 );
        std::string lobbyCodeString(lobbyCode);

        std::string command = commadType + lobbyCodeString;
        std::string serverMessage = makeServerMessage( command );
        client.send( serverMessage );
    };

    const MenuPage::FunctionList joinLobbyItemResults
        = {joinLobby, moveBackToMainMenuPage};

    MenuPage::MenuName joinLobbyName = "Join lobby"; 
    std::shared_ptr<MenuPage> joinLobbyPage = std::make_shared<MenuPage>( 
                                              joinLobbyName,
                                              joinLobbyFields,
                                              joinLobbyItems,
                                              joinLobbyItemResults );
    menuManager.addPage( joinLobbyPage );

    menuManager.setCurrentPage( mainMenuPage );
}
