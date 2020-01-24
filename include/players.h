#ifndef PLAYERS_H_
#define PLAYERS_H_

class Players{
public:
  Players();
  ~Players();
  void incrementPoints();
  void decrementPoints();
private:
  int playerID;
  int playersPoints;
};
Players(){
}
~Players(){
}
void incrementPoints(){
  playersPoints += 1000;
}
#endif
