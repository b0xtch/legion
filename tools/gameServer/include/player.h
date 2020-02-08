#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <utility>
// using std::unique_ptr
typedef int variable;
typedef std::string stringVar;
class Player{
public:
  Player(const variable&, const stringVar&);
  // void incrementPoints(const variable&);
  // void decrementPoints(const variable&);
  variable getPlayerID();
  // variable getPlayerPoints();
  friend bool operator== (const Player&, const Player&);
private:
  variable playerID;
  // variable playersPoints;
  stringVar playerName;
  networking::Connection connection; //Connections from PA, PB, PC to Game class for instantitations
};

#endif
