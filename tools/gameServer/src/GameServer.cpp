#include "GameServer.h"

GameServer::GameServer(int port) :
    server{port, "", onConnect, onDisconnect}
{
    
}

GameServer::~GameServer() {
    
}

void GameServer::send(const std::deque<networking::Message>& messages) {
    server.send(messages);
}

void GameServer::update() {
    
}

void GameServer::onConnect(networking::Connection c) {
    
}

void GameServer::onDisconnect(networking::Connection c) {
    
}