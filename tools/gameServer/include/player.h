#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <utility>
// using std::unique_ptr

using pID = int;
using pName = std::string;

class Player{
public:
  Player(pID, pName);
  void incrementPoints(int);
  void decrementPoints(int);
  pID& getPlayerID();
  pName& getPlayerName();
  int& getPlayerPoints();
  // friend bool operator== (const Player&, const Player&);
private:
  pID playerID;
  pName playerName;
  int playerPoints;
  // networking::Connection connection; //Connections from PA, PB, PC to Game class for instantitations
};

#endif
