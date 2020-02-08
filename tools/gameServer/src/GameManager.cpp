#include<iostream>
#include <vector>
#include "GameManager.h"


Game GameManager::createNewGame() {
    if (Games.size() < maxGames) {
        Game newgame();
        Games.push_back(newgame);
    }
}

void GameManager::startGame(const stringVar& name) {
  Games.emplace_back(std::make_unique<Game>("name", 8, "rules"));
  //specific Game entity will exist inside gameProcess only and destroyed when out of bounds of function

}
//endGame will kill the mentioned session
void GameManager::endGame(const stringVar& name)  {
  //Unfinished
  Games.erase(std::find_if(Games.begin(), Games.end(), Game(name)));
}
