#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <utility>
using std::unique_ptr

class Player{
public:
  Player(); //default
  Player(int pid, std::string name);
  ~Player();
  void incrementPoints(int x);
  void decrementPoints(int x);
  int getPlayerID();
  int getPlayerPoints();
  std::unique_ptr getPlayerTurn();
private:
  int playerID;
  int playersPoints;
  std::string playerName;
  std::unique_ptr playerTurn;
  networking::Connection connection; //Connections from PA, PB, PC to Game class for instantitations
};

#endif
