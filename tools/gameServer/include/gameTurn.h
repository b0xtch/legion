#ifndef GAME_TURN_H
#define GAME_TURN_H
#include "game.h"
#include "player.h"
//Every player object will have a game turn
//that can revolve around every time advanceGameTurn is called
class gameTurn{
public:
  gameTurn();
  ~gameTurn();
  void gamePlay(Game& sessionGamePlay); //every functionalities that happen during the game from json config
  std::string getPlayerInput();
  void setPlayerInput(); //storing player's input each turn to compare with "right answer" before computing points
  void startTurn();
  void endTurn();
private:
  std::string playerInput;
};

#endif
