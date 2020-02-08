#include "GameServer.h"

#include <algorithm>

#include "json.hpp"

#include <ctime>

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

void GameServer::sendTextTo(const std::vector<Connection>& connections, std::string text) {
    server.queueMessage(connections, text);
    server.sendQueuedMessages();
}

void GameServer::update() {
    server.update();
}

// HEAVY WIP TODO
std::string GameServer::receive() {
    auto incomingMessages = server.receive();
    std::ostringstream outgoingText;

    // for prepending each message with the time it was received
    time_t currentTime = time(0);
    tm *formatTime = localtime(&currentTime);
    
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
        else if (msgType == GameServer::MessageType::LeaveServer) {
            server.disconnect(c);
        }
        else { // If message is not a command it is sent to other clients
            outgoingText << "[" << formatTime->tm_hour << "] " << c.id << ": " << message.text << "\n";
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
    
    return outgoingText.str();

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
    else if (msgType.compare("quit")) {
        ret = GameServer::MessageType::LeaveServer;
    }
    else {
        ret = GameServer::MessageType::Other;
    }
    
    return ret;
}

int main(int argc, char* argv[]) {

    while (keepRunning) {
        try {
            server.update();
        } catch (std::exception& e) {
            keepRunning = false;
        }

        auto outgoing = receive();
        sendTextTo(clients, outgoing);
        sleep(1);
    }

    return 0;

}
