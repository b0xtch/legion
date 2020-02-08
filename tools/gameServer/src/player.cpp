#include <string>
#include <iostream>
#include "player.h"


Player::Player(const variable& pid, const stringVar& name): playerID(pid), playerName(name){}


void Player::incrementPoints(const variable& x){playerPoints += x;}

void Player::decrementPoints(const variable& x) {playerPoints -= x;}

variable Player::getPlayerID(){return playerID;}

variable Player::getPlayerPoints(){return playerPoints;}

bool operator== (const Player& player1, const Player& player2){ return player1->playerID == player2->playerID; }
