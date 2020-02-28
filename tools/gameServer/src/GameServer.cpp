#include "GameServer.h"

#include <algorithm>

#include <iostream>

#include "MessageType.h"
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

// PUBLIC

GameServer::GameServer(GameServerConfig gameServerConfig, unsigned short port, const std::string& htmlFile) :
    gameServerConfig{gameServerConfig},
    keepRunning{true},
    port{port},
    htmlFile{htmlFile},
    server{port, htmlFile,
        [this] (networking::Connection c) {
            this->sessionManager.addConnection(c);
        },
        [this] (networking::Connection c) {
            //this->sessionManager.removeConnection(c); Currently no implementation
        }},
    sessionManager{3}
{
    
}

GameServer::GameServer(GameServerConfig gameServerConfig, networking::Server& server, SessionManager& sessionManager) :
    gameServerConfig{gameServerConfig},
    server{std::move(server)},
    sessionManager{sessionManager},
    keepRunning{true}, port{0}, htmlFile{""}
{
    
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
        std::vector<networking::Message> toSend = sessionManager.processMessage(msg);
        batchToSend.insert(batchToSend.end(), toSend.begin(), toSend.end());
    }
    
    send(batchToSend);
}

unsigned short GameServer::getPort() const {
    return port;
}

bool GameServer::getKeepRunning() const {
    return keepRunning;
}

std::string_view GameServer::getHtmlFile() const {
    return htmlFile;
}
