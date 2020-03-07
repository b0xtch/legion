#ifndef GAME_H_
#define GAME_H_
#include <string>
#include <vector>
// #include "session.h"
#include "player.h"
#include "engine.h"
#include <json.hpp>

class Game: Engine{
public:
  Game(const stringVar&, const int& , const stringVar&); // general constructor to support additional games, will add more parameterss
  Game(const stringVar&);

  bool startGame();
  void addPlayer(const Player&);
  void removePlayer(const variable&);
  int getNumPlayers(); //interpreted from json config
  int getNumPlayersMax(); //interpreted from json config
  void playersInputs(); //playersInputs is the alternative to advanceGame since allPlayers inputs can be obtained all in one phase of the game
  std::vector<stringVar> getAllPlayersInputs();
  std::vector<Player> getAllPlayers();
  friend bool operator== (const Game&, const Game&);

private:
  int numPlayers; //interpreted from json config
  int numPlayersMax; //interpreted from json config
  std::vector<Player> allPlayers;
  std::vector<stringVar> allPlayersInputs; //store all the answer inputs from allPlayers
  stringVar gameName; //interpreted from json config
  stringVar rulesOfTheGame; //interpreted from json config, might be made into an object???
  bool audience; //interpreted from json config
};


#endif
