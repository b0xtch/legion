#include "GameServer.h"

#include <algorithm>

#include "MessageType.h"

GameServerConfig::GameServerConfig() :
    GameServerConfig{"./configuration.txt"}
{
    
}

GameServerConfig::GameServerConfig(const std::string& configLocation) :
    configLocation{configLocation}
{
    // TODO, load the file, parse it, and save it into some class variables
    // TODO, add error handling to this part
}

std::string_view GameServerConfig::getGameConfigDir() const {
    return gameConfigDir;
}

// PUBLIC

GameServer::GameServer(int port, const std::string& htmlFile) :
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
    sessionManager{}
{
    
}

GameServer::GameServer(networking::Server& server, SessionManager& sessionManager) :
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
    std::vector<networking::Message> unhandledMessages;
    for (auto& msg : incomingMessages) {
        auto& conn = msg.connection;
        auto msgType = MessageType::interpretType(msg.text);
        switch (msgType) {
            case MessageType::Type::ServerStop:
                this->keepRunning = false;
                break;
            case MessageType::Type::CreateSession:
                this->sessionManager.createNewSession();
                break;
            case MessageType::Type::JoinSession:
                this->sessionManager.addToSession(conn, ""); Currently no implementation
                break;
            case MessageType::Type::LeaveServer:
                server.disconnect(c);
                break;
            default:
                unhandledMessages.push_back(msg);
                break;
        }
    }
    
    // Pass the remaining messages to SessionManager for distribution and handling
    //auto toSendOut = sessionManager.process(unhandledMessages); Currently no implementation
    
    //send(toSendOut);
}

void GameServer::update() {
    server.update();
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
