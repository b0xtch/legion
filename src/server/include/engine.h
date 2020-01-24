#ifndef ENGINE_H
#define ENGINE_H
#include "game.h";

class Engine {
private:
    Game game;

public:
    Engine();
    ~Engine();

    void start();

};

#endif