#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include "game.h"
#include "player.h"
#include "engine.h"

class GameManager {

public:
    GameManager();

    Game createNewGame();
    void startGame();
    void endGame();
    void advanceGameTurn();

private:
    Engine gameEngine;
    std::vector<Game> Games;

};

#endif
