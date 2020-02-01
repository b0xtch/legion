#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include "Server.h"
#include "SessionManager.h"


class GameServer {
public:
    GameServer(int port);
    
    /** Sends out all the messages passed to the intended clients. */
    void send(const std::deque<networking::Message>& messages);
    
    /** Allows the server to queue new incoming messages to receive them. */
    void update();
    
    /** Allows the server to update games, sessions, and basically work. */
    void receive();
    
private:
    int port;
    networking::Server server;
    SessionManager sessionManager;
};

#endif