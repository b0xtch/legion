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
    
    /** Sends out all the messages passed to the intended clients. */
    void send(const std::deque<networking::Message>& messages);
    
    /** Allows the server to receive messages, send messages, process ongoing games, and basically work. */
    void update();

private:
    /** Called when an initial connection is made to the server. */
    void onConnect(networking::Connection c);
    
    /** Called when an existing connection disconnects from the server. */
    void onDisconnect(networking::Connection c);
    
    int port;
    networking::Server server;
    // todo SessionManager sessionManager
};

#endif