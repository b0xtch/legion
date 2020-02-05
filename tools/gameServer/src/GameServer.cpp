#include "GameServer.h"

#include <algorithm>

#include "json.hpp"

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
        
        auto msgType = parseMessageType(message.text);
        
        if (msgType == GameServer::MessageType::ServerStop) {
            keepRunning = false;
        }
        else if (msgType == GameServer::MessageType::CreateSession) {
            sessionManager.createNewSession();
        }
        else if (msgType == GameServer::MessageType::JoinSession) {
            sessionManager.joinToSession();
        }
    }
    
    auto it = std::remove_if(incomingMessages.front(), incomingMessages.back(), 
        [] (networking::Message msg) {
            auto msgType = parseMessageType(msg.text);
            return msgType != GameServer::MessageType::Other;
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

// ==================================================

// PRIVATE

GameServer::MessageType GameServer::parseMessageType(std::string text) {
    using json = nlohmann::json;
    
    json jsonObj = json::parse(text);
    
    // Message format is currently unknown so some placeholder strings are in place.
    std::string msgType = jsonObj["type"];
    
    GameServer::MessageType ret;
    if (msgType.compare("serverStop")) {
        ret = GameServer::MessageType::ServerStop
    }
    else if (msgType.compare("create")) {
        ret = GameServer::MessageType::CreateSession;
    }
    else if (msgType.compare("join")) {
        ret = GameServer::MessageType::JoinSession;
    }
    else {
        ret = GameServer::MessageType::Other;
    }
    
    return ret;
}