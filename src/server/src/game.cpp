#include <iostream>
#include <string>
#include <vector>

#include "game.h"
#include "players.h"

using namespace std;

Game::Game(){
  gameName = "Jackbox no TV";
  numPlayersMax = 8;
  numPlayers = 0; //setting to 0 so that the client server connection can determine number of players connecting
  rulesOfTheGame = "Welcome to Jackbox no TV, the rules of the game are ...";
}

//general constructor -- WIP
Game::Game(std::string name, int maxPlayers, std::string rules){
  gameName = name;
  numPlayersMax = maxPlayers;
  numPlayers = 0;
  rulesOfTheGame = rules;
}

Game::~Game(){
  for(std::string* question: questions){ delete[] question; }
  questions.clear();
  std::cout<<"Deleted vector questions, size of vector = "<<questions.size()<<endl;
  allPlayers.clear();
  std::cout<<"Deleted vector allPlayers, size of vector = "<<allPlayers.size()<<endl;
}

void Game::addPlayer(Players p){
	if(numPlayers != numPlayersMax){
		allPlayers.push_back(p);
		numPlayers++;
	} else {
		std::cout << "Game currently at maximum capacity. Player (" << p.getPlayerID() << ") unable to join. \n";
	}	
}

void Game::removePlayer(int pid){
	//placeholder - looking for alternative solutions for organizing vector of players
	for(int i = 0; i<numPlayers; i++){
		if(allPlayers[i].getPlayerID() == pid){
			allPlayers.erase(allPlayers.begin() + i);
			numPlayers--;
		}
	}
}

int Game::getNumPlayers(){
	return numPlayers;
}
int Game::getNumPlayersMax(){
	return numPlayersMax;
}



/*
*  WIP
*/

// std::string Game::rightAnswerForQuestion(const std::vector<std::string*>& questions){
// 	return ""; // find best way to match question with answers
// }

// void addQuestions(std::string question, std::string answer){

// }