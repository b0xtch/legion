#include <string>
#include <iostream>
#include "player.h"


Player::Player(pID id, pName name): 
	playerID(id), 
	playerName(name),
	playerPoints(0)
	{};

// Player::Player() : playerID(0), playerName("not_initialized") {};

void Player::incrementPoints(int x){playerPoints += x;}

void Player::decrementPoints(int x){playerPoints -= x;}

pID& Player::getPlayerID(){return playerID;}
pName& Player::getPlayerName(){return playerName;}

int& Player::getPlayerPoints(){return playerPoints;}

// bool operator== (const Player& player1, const Player& player2){ return player1->playerID == player2->playerID; }
