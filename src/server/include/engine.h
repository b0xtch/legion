#ifndef ENGINE_H
#define ENGINE_H
#include "game.h"
#include <nlohmann/json.hpp>

/**
 * Parses the json files that will be used to create
 * games dynamically
 */

class Engine {
public:
    Engine();
    ~Engine();

    void start();
private:
    Game game;
    nlohmann::json config;
};

#endif