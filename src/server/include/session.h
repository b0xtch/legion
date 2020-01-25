#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "game.h"
#include "player.h"

class Session {
public:
    Session(int ID, Game& game, Player& sessionCreator);
    ~Session();

    void updateTime();
    void checkTimeOut();

    void startGame();
    void endGame();
    void advanceGameTurn();
    void declareWinner();

    void addPlayer(Player& newPlayer);
    void removePlayer(int removePlayerID);

private:
    Game sessionGame;
    std::vector<Player> sessionPlayers;

    int sessionID;
    double maxIdleTime = 30;
    std::chrono::system_clock::time_point lastUpdateTime;
    
};

#endif
