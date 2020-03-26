#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include <sstream>
#include <memory>
#include "json.hpp"
#include <iostream>

#include "MenuPage.h"
#include "MenuManager.h"
#include "ChatWindow.h"
#include "Client.h"
#include "Utils.h"
#include "ParsedMessage.h"

#include <thread>
#include <chrono>

using json = nlohmann::json;

// Basic menu structure referenced from:
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html

// Basic menu structure referenced from:
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/forms.html

// Idea on combining menus and forms on one page referenced from:
//    https://alan-mushi.github.io/2015/05/26/simple-ncurses-popup-in-C.html


void initializeMenuPages( MenuManager &menuManager, bool &done,
                            networking::Client &client );

// Split a string into multiple substrings
std::vector<std::string> splitString(const std::string& text, const std::string& splitOn, bool includeSplit);
// Make json message for server
std::string makeServerMessage(const std::string& input);
// Parse json message from server
std::string processServerMessage(const std::string& response);
// Get the current client games list (for testing purposes)
std::string getGamesList();

std::vector<std::string> *gamesList = nullptr; // A list of games on the server, updated when the client sends a !gamerequest command

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
            std::vector<std::string> commands = splitString(response, "}", true);

            for ( auto cmd : commands ) {
                menuManager.displayChatText(processServerMessage(cmd));
            }
        }

        menuManager.update();

    }

    menuManager.cleanup();

    return 0;
}

/**
Takes a string and a second string to split the first string on.
Splits the first string and returns a list of all the new substrings.
The substrings will include the next split string if includeSplit is true.
**/
std::vector<std::string> splitString(const std::string& text, const std::string& splitOn, bool includeSplit) {

    std::vector<std::string> splits;

    if (!text.empty()) {
        std::size_t start = 0;
        std::size_t end = text.find(splitOn);

        while (end != std::string::npos) {
            if (includeSplit) {
                splits.push_back(text.substr(start, end-start+splitOn.length()));
            } else {
                splits.push_back(text.substr(start, end-start));
            }
            start = end + splitOn.length();
            end = text.find(splitOn, start);
        }

        splits.push_back(text.substr(start));
    }

    return splits;
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
        std::vector<std::string> newGamesList = splitString(data, "\n", false);
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

void initializePages(MenuManager &menuManager, bool &done, networking::Client &client) {
    // Build page for creating lobby
    // Build page for joining lobby
    // Build page for main menu
    // Set the current page to main menu
}

MenuPageInfo::MenuName buildJoinLobbyPage(MenuManager &menuManager, networking::Client& client) {
    
    // Join lobby menu
    const MenuPageInfo::NameList joinLobbyFields = {"Lobby code:"};

    const MenuPageInfo::NameList joinLobbyItems = {"Join", "Back"};

    // Join lobby item functions
    auto moveBackToMainMenuPage = [&menuManager] () {
        MenuPageInfo::MenuName nextPage = "Main menu";
        menuManager.switchPage( nextPage );
    };

    auto joinLobby = [&client, &menuManager] () {
        std::shared_ptr<MenuPage> joinPage = menuManager.getCurrentPage();
        form_driver(joinPage->getForm(), REQ_VALIDATION);
        
        MenuPage::FieldList *connectFields = joinPage->getFieldList();

        const int lobbyCodeInputIndex = 1;
        const char *lobbyCode =
            field_buffer( connectFields->at( lobbyCodeInputIndex ), 0 );
        std::string lobbyCodeString(lobbyCode);
        // Bug: if field is not filled to max length, only spaces will be sent.
        // What's happening: https://alan-mushi.github.io/2014/11/30/ncurses-forms.html
        // Fix: https://stackoverflow.com/questions/18493449/how-to-read-an-incomplete-form-field-ncurses-c
        std::string serverMessage = ParsedMessage::makeMsgText(PMConstants::TYPE_JOIN_SESSION, lobbyCodeString);
        client.send( serverMessage );
    };

    const MenuPageInfo::FunctionList joinLobbyItemResults
        = {joinLobby, moveBackToMainMenuPage};

    MenuPageInfo::MenuName joinLobbyName = "Join lobby";
    auto joinLobbyPage = std::make_shared<MenuPageInfo>(joinLobbyName, joinLobbyFields, joinLobbyItems, joinLobbyItemResults );
    menuManager.addPage( joinLobbyPage );
    
    return "Join lobby";
}

std::string requestGames(networking::Client& client) {
    std::string serverMessage = ParsedMessage::makeMsgText(PMConstants::TYPE_REQUEST_GAMES, "");
    client.send(serverMessage);
    
    client.update();
    auto response = client.receive();
    while ( response.empty() ) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        client.update();
        response = client.receive();
    }
    
    ParsedMessage pm = ParsedMessage::interpret(response);
    
    if (pm.getType() == ParsedMessage::Type::RequestGames) {
        return pm.getData();
    }
    else {
        return "<no games>";
    }
}

void displayGames(MenuManager& menuManager, const std::string& games) {
    std::vector<std::string> gamesList = splitString(games, "\n", false);
    std::stringstream ss{};
    ss << "--- GAME TITLES\n";
    for (auto& game : gamesList) {
        ss << game << "\n";
    }
    ss << "---\n";
    
    menuManager.displayChatText(ss.str());
}

MenuPageInfo::MenuName buildCreateLobbyPage(MenuManager &menuManager, networking::Client& client) {
    
    const MenuPageInfo::NameList createLobbyFields = {"Game title:"};

    const MenuPageInfo::NameList createLobbyItems = {"Create", "Back"};

    // Join lobby item functions
    auto moveBackToMainMenuPage = [&menuManager] () {
        MenuPageInfo::MenuName nextPage = "Main menu";
        menuManager.switchPage( nextPage );
    };

    auto createLobby = [&client, &menuManager] () {
        std::shared_ptr<MenuPage> createPage = menuManager.getCurrentPage();
        form_driver(createPage->getForm(), REQ_VALIDATION);
        
        MenuPage::FieldList *connectFields = createPage->getFieldList();

        const int gameTitleInputIndex = 1;
        const char *gameTitle =
            field_buffer( connectFields->at( gameTitleInputIndex ), 0 );
        std::string gameTitleString(gameTitle);
        // Bug: if field is not filled to max length, only spaces will be sent.
        // What's happening: https://alan-mushi.github.io/2014/11/30/ncurses-forms.html
        // Fix: https://stackoverflow.com/questions/18493449/how-to-read-an-incomplete-form-field-ncurses-c
        std::string serverMessage = ParsedMessage::makeMsgText(PMConstants::TYPE_CREATE_SESSION, gameTitleString);
        client.send( serverMessage );
    };

    const MenuPageInfo::FunctionList createLobbyItemResults
        = {createLobby, moveBackToMainMenuPage};

    MenuPageInfo::MenuName createLobbyName = "Create lobby";
    auto createLobbyPage = std::make_shared<MenuPageInfo>(createLobbyName, createLobbyFields, createLobbyItems, createLobbyItemResults );
    menuManager.addPage( createLobbyPage );
    
    return "Create lobby";
}

void initializeMenuPages( MenuManager &menuManager, bool &done,
                            networking::Client &client ) {

    // Main menu
    MenuPageInfo::NameList mainMenuFields = {};

    MenuPageInfo::NameList mainMenuItems = { "Join lobby", "Create lobby", "Exit" };

    MenuPageInfo::MenuName joinLobbyMenuName = buildJoinLobbyPage(menuManager, client);
    MenuPageInfo::MenuName createLobbyMenuName = buildCreateLobbyPage(menuManager, client);

    // Main menu item functions
    auto moveToJoinLobbyPage = [&menuManager, joinLobbyMenuName] () {
        MenuPageInfo::MenuName nextPage = joinLobbyMenuName;
        menuManager.switchPage( nextPage );
    };
    
    auto moveToCreateLobbyPage = [&menuManager, &client, createLobbyMenuName] () {
        std::string response = requestGames(client);
        displayGames(menuManager, response);
        
        MenuPageInfo::MenuName nextPage = createLobbyMenuName;
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

    menuManager.setCurrentPage( mainMenuPage->menuName );
}
