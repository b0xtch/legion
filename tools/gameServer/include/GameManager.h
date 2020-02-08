#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include "game.h"
#include "engine.h"
#include "player.h"

typedef std::unique_ptr<Game> gamePtr;
class GameManager {
public:
    GameManager();

    Game createNewGame(); //For now, startGame is managing lifetimes of all the Game objects
    void startGame(stringVar name); //startGame will have a vector of gamePtr to manage Game objects' lifetimes
    void endGame(stringVar name);
// advanceGame is not needed since most of the games don't need to revolve around turns
private:
    int maxGames;
    Engine gameEngine;
    std::vector<gamePtr> Games;
};

#endif
