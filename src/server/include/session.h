#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "game.h"
#include "players.h"

class Session {
public:
    Session();
    ~Session();

    void updateTime();
    void checkTimeOut();

    void startGame();
    void endGame();
    void advanceGameTurn();
    void declareWinner();

private:
    Game sessionGame;
    std::vector<Players> sessionPlayers;

    int sessionID;
    double maxIdleTime = 30;
    std::chrono::system_clock::time_point lastUpdateTime;
    
};

#endif
