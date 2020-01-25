#ifndef ENGINE_H
#define ENGINE_H
#include "game.h"
#include <nlohmann/json.hpp>

/**
 * Parses the json files that will be used to create
 * games dynamically
 */

class Engine {
private:
    Game game;

public:
    Engine();
    ~Engine();

    void start();

};

#endif