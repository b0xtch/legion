#include "GameServer.h"

#include <algorithm>

#include <iostream>

#include "MessageType.h"
#include "json.hpp"
#include "Utils.h"

GameServerConfig::GameServerConfig() :
    GameServerConfig{"./configuration.txt"}
{
    
}

GameServerConfig::GameServerConfig(const std::string& configLocation) :
    configLocation{configLocation}
{
    using json = nlohmann::json;
    
    json j;
    try {
        j = json::parse(Utils::loadFile(configLocation));
        gameDir = j[CFGKEY_GAME_DIR];
    }
    catch (const json::parse_error& e) {
        std::cerr << "There was a problem reading the configuration file." << std::endl;
    }
    catch (const json::type_error& e) {
        std::cerr << "There are missing configurations in the file." << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "There was an error opening the configuration file." << std::endl;
    }
}

std::string_view GameServerConfig::getGameConfigDir() const {
    return gameDir;
}

// PUBLIC

GameServer::GameServer(GameServerConfig gameServerConfig, int port, const std::string& htmlFile) :
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
    keepRunning{true}, port{-1}, htmlFile{""}
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

int GameServer::getPort() const {
    return port;
}

bool GameServer::getKeepRunning() const {
    return keepRunning;
}

std::string_view GameServer::getHtmlFile() const {
    return htmlFile;
}
