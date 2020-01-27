#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "Server.h"
#include "engine.h"

#include <iostream>
#include <string>
#include <vector>

class gameServer {
public:
    gameServer();
    ~gameServer();

    void start();
    void shutdown();

    void onConnect();
    void onDisconnect();
    void receiveMessage();
    void sendMessage();

private:
    Engine engn;
};

#endif