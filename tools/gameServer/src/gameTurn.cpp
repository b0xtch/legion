#include <iostream>
#include "gameTurn.h"
using namespace std;

gameTurn::gameTurn(){}

gameTurn::~gameTurn(){}

std::string gameTurn::getPlayerInput(){
  return playerInput;
}
void gameTurn::setPlayerInput(){
  std::string input = null;
  std::cin >> input;
  playerInput = input;
}
void gameTurn::startTurn(){
 //Unfinished
}

void gameTurn::endTurn(){
  //Unfinished
}
