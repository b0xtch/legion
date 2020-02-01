#include "GameServer.h"

void onConnect(networking::Connection c);
void onDisconnect(networking::Connection c);

GameServer::GameServer(int port) :
    port{port},
    server{port, "", onConnect, onDisconnect}
{
    
}

void GameServer::send(const std::deque<networking::Message>& messages) {
    server.send(messages);
}

void GameServer::receive() {
    auto incomingMessages = server.receive();
    // Pass these messages to SessionManager for distribution and handling?
}

void GameServer::update() {
    server.update();
    
}

/** Called when an initial connection is made to the server. */
void onConnect(networking::Connection c) {
    // Initial connection. Need to hold onto this connection so we can deal with incoming create game/join game requests.
    sessionManager.addNewConnection(c);
}

/** Called when an existing connection disconnects from the server. */
void onDisconnect(networking::Connection c) {
    // 
}