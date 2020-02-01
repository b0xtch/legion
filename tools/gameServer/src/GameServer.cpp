#include "GameServer.h"

GameServer::GameServer(int port) :
    port{port},
    server{port, "", onConnect, onDisconnect}
{
    
}

void GameServer::send(const std::deque<networking::Message>& messages) {
    server.send(messages);
}

void GameServer::update() {
    server.update();
    auto incomingMessages = server.receive();
    // Pass these messages to SessionManager for distribution and handling?
}

void GameServer::onConnect(networking::Connection c) {
    // Initial connection. Need to hold onto this connection so we can deal with incoming create game/join game requests.
}

void GameServer::onDisconnect(networking::Connection c) {
    // 
}