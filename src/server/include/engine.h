#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include "game.h"
#include <nlohmann/json.hpp>

/**
 * Parses the json files that will be used to create
 * games dynamically
 */

struct Mode {
    // something else that would allow the strcut more abstraction
    std::string gameMode;
};

class Engine {
public:
    Engine(/*add some params here on initalization*/);
    ~Engine();

    void newGame(nlohmann::json& config, Mode mode);

    void start(Game g&);
    void stop(Game g&);

private:
    Game game;
    nlohmann::json config;

    nlohmann::json getConfig(Game g&) const;
    void setConfig(nlohmann::json& config) const;
};

#endif
