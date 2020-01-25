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

    void newGame(nlohmann::json& config, std::string mode);

    void start(Game g&);
    void stop(Game g&);

private:
    std::vector<Game> games;
    nlohmann::json config;

    nlohmann::json getConfig(Game g&) const;
    void setConfig(nlohmann::json& config) const;
};

#endif