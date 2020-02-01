#include "GameServer.h"

using json = nlohmann::json;

GameServer::GameServer(int port, std::string htmlFile) :
    keepRunning{true},
    port{port},
    htmlFile{htmlFile},
    server{port, htmlFile,
        [this] (networking::Connection c) {
            this->sessionManager.addConnection(c);
        },
        [this] (networking::Connection c) {
            this->sessionManager.removeConnection(c);
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
        
        auto& jsonText = json::convertToJson(message.text);
        auto typeVal = jsonText.get(MessageKey.Type);
        
        if (typeVal == MessageType.ServerStop) {
            keepRunning = false;
        }
        else if (typeVal == MessageType.CreateSession) {
            sessionManager.createNewSession();
        }
        else if (typeVal == MessageType.JoinSession) {
            sessionManager.joinToSession();
        }
    }
    
    auto it = std::remove_if(incomingMessages.front(), incomingMessages.back(), 
        [] (networking::Message msg) { 
            auto typeVal = json::convertToJson(msg.text).get(MessageKey.Type);
            return typeVal == MessageType.CreateSession || typeVal == MessageType.JoinSession;
        }
    );
    
    // Pass these messages to SessionManager for distribution and handling?
    sessionManager.process(incomingMessages);
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