#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include <string>
#include <vector>
#include "game.h"
#include "players.h"

class Session {
public:
    Session();
    ~Session();

    void startGame();
    void endGame();
    void advanceGameTurn();
    void declareWinner();

private:
    Game game;
    std::vector<Players> players;
};

#endif
