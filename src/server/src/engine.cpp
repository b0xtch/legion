#include "engine.h"
#include <iostream>

using json = nlohmann::json;

Engine::Engine() {}

Engine::~Engine() {}

void Engine::newGame(nlohmann::json& config, std::string mode) {}

void Engine::start(Game g&) {}

void Engine::stop(Game g&) {}

nlohmann::json Engine::getConfig(Game g&) {}

void Engine::setConfig(nlohmann::json& config) {}