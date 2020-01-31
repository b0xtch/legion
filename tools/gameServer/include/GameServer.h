#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "Server.h"

#include <iostream>
#include <string>
#include <vector>

class GameServer {
public:
    GameServer(int port);
    ~GameServer();
    void send(const std::deque<networking::Message>& messages);
    void update();

private: 
    void onConnect(networking::Connection c);
    void onDisconnect(networking::Connection c);
    
    networking::Server server;
    
};

#endif