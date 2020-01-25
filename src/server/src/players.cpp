#include <string>

#include "players.h"

Players::Players(){
	playerID = 0; // should not be 0, will figure out how to handle
	playerName = "noname";
	playersPoints = 0;
}

Players::Players(int pid, std::string name){
	playerID = pid;
	playerName = name;
	playersPoints = 0;
}

Players::~Players(){
}

void Players::incrementPoints(int x){
 	playersPoints += x;
}

void Players::decrementPoints(int x){
	playersPoints -= x;
}

int Players::getPlayerID(){
	return playerID;
}
int Players::getPlayerPoints(){
	return playersPoints;
}
