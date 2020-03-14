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
#include "Utils.h"

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
// Get the current client games list (for testing purposes)
std::string getGamesList();

std::std::vector<std::string> *gamesList = nullptr; // A list of games on the server, updated when the client sends a !gamerequest command

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

std::string makeServerMessage(const std::string& input) {

    json message = Utils::makeJsonCommand(input);
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

        /**
        The list of games on the server is given as a string, separated by new lines.
        The following code separates the string into individual games and updates the global gamesList vector.
        **/
        std::vector<std::string> newGamesList;

        if (!data.empty()) {
            std::size_t start = 0;
            bool endOfData = false;

            while (!endOfData) {
                std::size_t end = data.find(start, "\n");
                if (end == string::npos) {
                    endOfData = true;
                } else {
                    newGamesList.push_back(data.substr(start, end-start));
                    start += 2;
                }
            }

            newGamesList.push_back(data.substr(start);
        }

        gamesList = &newGamesList;

    }

    return responseData.str();

}

std::string getGamesList() {

    std::stringstream games;

    if (gamesList != nullptr) {
        for (auto game : *gamesList) {
            games << game << " ";
        }
    }

    return games.str();

}

void initializeMenuPages( MenuManager &menuManager, bool &done,
                            networking::Client &client ) {

    // Main menu
    MenuPageInfo::NameList mainMenuFields = {};

    MenuPageInfo::NameList mainMenuItems = { "Join lobby", "Create lobby", "Exit" };

    // Main menu item functions
    auto moveToJoinLobbyPage = [&menuManager] () {
        MenuPageInfo::MenuName nextPage = "Join lobby";
        menuManager.switchPage( nextPage );
    };

    auto moveToCreateLobbyPage = [&client, &menuManager] () {

        // Get the list of games

        std::string commadType =  "!requestgames ";
        std::string serverMessage = makeServerMessage( commadType );
        client.send( serverMessage );

        auto response = client.receive();
        //while ( response.empty() ) {
        //    response = client.receive();
        //}

        MenuPageInfo::NameList gamesList = {"Game 1", "Game 2"};
            /* TEMP create list from response */

        // Define the function that is called when a game is chosen

        auto chooseGame = [&client, &gamesList, &menuManager] () {

            //std::string gameChoice = gamesList[menuManager.getSelectedIndex()];
            //client.send( gameChoice );

        };

        // Create the "Creat lobby" menu page with the given games

        MenuPageInfo::NameList createLobbyFields = {};

        MenuPageInfo::NameList createLobbyItems = {};

        for ( auto gameName : gamesList ) {
            createLobbyItems.push_back( gameName );
        }
        MenuPageInfo::ItemName backString = "back";
        createLobbyItems.push_back( backString );

        MenuPageInfo::FunctionList createLobbyItemResults = {};

        for ( int i = 0; i < gamesList.size(); i++ ) {
            createLobbyItemResults.push_back( chooseGame );
        }

        auto moveBackToMainMenuPage = [&menuManager] () {
            MenuPageInfo::MenuName nextPage = "Main menu";
            menuManager.switchPage( nextPage );
        };
        createLobbyItemResults.push_back( moveBackToMainMenuPage );

        MenuPageInfo::MenuName createLobbyName = "Create lobby";
        std::shared_ptr<MenuPageInfo> createLobbyPage = std::make_shared<MenuPageInfo>(
                                                    createLobbyName,
                                                    createLobbyFields,
                                                    createLobbyItems,
                                                    createLobbyItemResults );
        menuManager.addPage( createLobbyPage );

        // The games are all loaded and the page is setup, so switch to the page

        MenuPageInfo::MenuName nextPage = "Create lobby";
        menuManager.switchPage( nextPage );
    };

    auto exitProgram = [&done] () {
        done = true;
    };

    MenuPageInfo::FunctionList mainMenuItemResults = { moveToJoinLobbyPage,
                                                   moveToCreateLobbyPage,
                                                   exitProgram };

    MenuPageInfo::MenuName mainMenuName = "Main menu";
    std::shared_ptr<MenuPageInfo> mainMenuPage = std::make_shared<MenuPageInfo>(
                                             mainMenuName,
                                             mainMenuFields,
                                             mainMenuItems,
                                             mainMenuItemResults );
    menuManager.addPage( mainMenuPage );



    // Join lobby menu
    const MenuPageInfo::NameList joinLobbyFields = {"Lobby code:"};

    const MenuPageInfo::NameList joinLobbyItems = {"Join", "Back"};

    // Join lobby item functions

    auto moveBackToMainMenuPage = [&menuManager] () {
        MenuPageInfo::MenuName nextPage = "Main menu";
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

    const MenuPageInfo::FunctionList joinLobbyItemResults
        = {joinLobby, moveBackToMainMenuPage};

    MenuPageInfo::MenuName joinLobbyName = "Join lobby";
    std::shared_ptr<MenuPageInfo> joinLobbyPage = std::make_shared<MenuPageInfo>(
                                              joinLobbyName,
                                              joinLobbyFields,
                                              joinLobbyItems,
                                              joinLobbyItemResults );
    menuManager.addPage( joinLobbyPage );

    menuManager.setCurrentPage( mainMenuPage->menuName );
}
