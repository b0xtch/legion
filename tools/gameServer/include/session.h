#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "game.h"
#include "player.h"

// class Game;
class Session {
  // friend class Game;
public:
    Session(int ID, Game& game, Player& sessionCreator);
    ~Session();

    void updateTime();
    void checkTimeOut();

    void startGame();
    void endGame();
    void advanceGameTurn(); //state machine???
    void declareWinner();

    void addPlayer(Player& newPlayer);
    void removePlayer(int removePlayerID);

private:
    Game sessionGame; //each session should only handle 1 Game at a time for now
    std::vector<Player> sessionPlayers;

    int sessionID;
    double maxIdleTime = 30;
    std::chrono::system_clock::time_point lastUpdateTime;

};

#endif
