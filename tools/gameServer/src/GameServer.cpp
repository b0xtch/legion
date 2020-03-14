#include "GameServer.h"

#include <algorithm>

#include <iostream>
#include <sstream>

#include "ParsedMessage.h"
#include "json.hpp"
#include "Utils.h"

GameServerConfig::GameServerConfig() :
    gameDir{"games/"}, maxSessions{10}, maxConnections{100}
{
    
}

void GameServerConfig::parse(const std::string& configText) {
    using json = nlohmann::json;
    
    try {
        json j = json::parse(configText);
        gameDir = j[CFGKEY_GAME_DIR].get<std::string>();
        maxSessions = j[CFGKEY_MAX_SESSIONS].get<int>();
        maxConnections = j[CFGKEY_MAX_CONNECTIONS].get<int>();
        
        // NOTE: Be careful when using "." or ".." as the value for "games" in the configuration file.
        // Those will refer to the working directory of the program rather than the location of the config file.
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("There was a problem reading the configuration data.");
    }
    catch (const json::type_error& e) {
        throw std::runtime_error("There are missing configurations.");
    }
    catch (const std::runtime_error& e) {
        throw std::runtime_error("There was an error opening the configuration file.");
    }
}

std::string GameServerConfig::getGameConfigDir() const {
    return gameDir;
}

int GameServerConfig::getMaxSessions() const {
    return maxSessions;
}

int GameServerConfig::getMaxConnections() const {
    return maxConnections;
}

GameServer::GameServer(GameServerConfig gameServerConfig, unsigned short port) :
    gameServerConfig{gameServerConfig},
    keepRunning{true},
    port{port},
    server{port, "",
        [this] (networking::Connection c) {
            this->sessionManager.addConnection(c);
        },
        [this] (networking::Connection c) {
            this->sessionManager.removeConnection(c);
        }},
    sessionManager{gameServerConfig.getMaxSessions()}
{
    fillGameFilesMap();
}

void GameServer::send(const std::deque<networking::Message>& messages) {
    server.send(messages);
}

void GameServer::update() {
    server.update();
}

void GameServer::receive() {
    auto incomingMessages = server.receive();
    
    // Check and deal with messages about creating/joining rooms or server shutdowns.
    std::deque<networking::Message> batchToSend{};
    
    for (auto& msg : incomingMessages) {
        std::cout << "[GameServer] RECV " << msg.connection.id << ": " << msg.text << std::endl;
        
        // If message about requesting the list of games or server shutdown, deal with it. Direct the rest to sessionManager.
        ParsedMessage pMsg = ParsedMessage::interpret(msg.text);
        
        switch (pMsg.getType()) {
            case ParsedMessage::Type::ListGames:
                // WIP loop through all files from gameServerConfig's gameDir and format it into a message to send back.
                batchToSend.push_back(generateGameListResponse(msg.connection));
                break;
            case ParsedMessage::Type::ServerStop:
                keepRunning = false;
                break;
            default:
                auto sessMgrMsgs = sessionManager.processMessage(msg);
                batchToSend.insert(batchToSend.end(), sessMgrMsgs.begin(), sessMgrMsgs.end());
                break;
        }
    }
    
    send(batchToSend);
}

unsigned short GameServer::getPort() const {
    return port;
}

bool GameServer::getKeepRunning() const {
    return keepRunning;
}

networking::Message GameServer::generateGameListResponse(networking::Connection recipient) {
    networking::Message msg;
    msg.connection = recipient;
    
    std::stringstream msgContent;
    
    // The type of "name" is std::pair<std::string gameName, std::string gamePath>
    for (auto& name : gameNameToPathMap) {
        if (name == *gameNameToPathMap.begin()) {
            msgContent << name.first;
        }
        else {
            msgContent << "\n" << name.first;
        }
    }
    msg.text = ParsedMessage::makeMsgText(ParsedMessage::Type::ListGames, msgContent.str());
    return msg;
}

void GameServer::fillGameFilesMap() {
    std::vector<std::string> files = Utils::listFiles(gameServerConfig.getGameConfigDir());
    for (auto& filename : files) {
        try {
            std::string name = Utils::getGameName(filename);
            // Only add a gamename-to-file mapping if the gamename doesn't already appear in the map.
            if (gameNameToPathMap.count(name) == 0) {
                gameNameToPathMap[name] = filename;
            }
            else {
                std::cerr << "Game with name \"" + name + "\" already appears in the game list. Cannot add game in \"" << filename << "\"." << std::endl;
            }
        }
        catch (std::runtime_error& e) {
            std::cerr << "File \"" + filename + "\" does not appear to be a valid game file." << std::endl;
        }
    }
}
