#include <string>

#include "player.h"

Player::Player(){
	playerID = 0; // should not be 0, will figure out how to handle
	playerName = "noname";
	playerPoints = 0;
}

Player::Player(int pid, std::string name){
	playerID = pid;
	playerName = name;
	playerPoints = 0;
}

Player::~Player(){
}

void Player::incrementPoints(int x){
 	playerPoints += x;
}

void Player::decrementPoints(int x){
	playerPoints -= x;
}

int Player::getPlayerID(){
	return playerID;
}
int Player::getPlayerPoints(){
	return playerPoints;
}
