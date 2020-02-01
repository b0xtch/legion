#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include "game.h"
#include "engine.h"
#include "player.h"

class GameManager {

public:
    GameManager();

    Game createNewGame();
    void startGame(std::string name);
    void endGame(std::string name);
    void advanceGame(std::string name);

private:
    int maxGames;
    Engine gameEngine;
    std::vector<Game> Games;

};

#endif

