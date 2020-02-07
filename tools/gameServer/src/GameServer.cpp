#include "GameServer.h"

#include <algorithm>

#include "MessageType.h"

// PUBLIC

GameServer::GameServer(int port, std::string htmlFile) :
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

void GameServer::send(const std::deque<networking::Message>& messages) {
    server.send(messages);
}

// HEAVY WIP TODO
void GameServer::receive() {
    auto incomingMessages = server.receive();
    
    // Check for messages about creating or joining a room.
    for (auto& message : incomingMessages) {
        auto& c = message.connection;
        
        auto msgType = MessageType::interpretType(message.text);
        
        if (msgType == MessageType::Type::ServerStop) {
            keepRunning = false;
        }
        else if (msgType == MessageType::Type::CreateSession) {
            sessionManager.createNewSession();
        }
        else if (msgType == MessageType::Type::JoinSession) {
            //sessionManager.joinToSession(); Currently no implementation
        }
    }
    
    auto it = std::remove_if(incomingMessages.front(), incomingMessages.back(), 
        [] (networking::Message msg) {
            auto msgType = MessageType::interpretType(msg.text);
            return msgType != MessageType::Type::Other;
        }
    );
    
    // Pass these messages to SessionManager for distribution and handling?
    //sessionManager.process(incomingMessages); Currently no implementation
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
