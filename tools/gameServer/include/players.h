#ifndef PLAYERS_H_
#define PLAYERS_H_

#include <string>

class Players{
public:
  Players(); //default
  Players(int pid, std::string name);
  ~Players();
  void incrementPoints(int x);
  void decrementPoints(int x);
  int getPlayerID();
  int getPlayerPoints();
private:
  int playerID;
  int playersPoints;
  std::string playerName;
};

#endif
