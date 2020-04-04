#include <iostream>
#include <string>
#include <vector>
#include "game.h"
#include "player.h"
#include <json.hpp>

using namespace std;
using json = nlohmann::json;
using Engine::Game;

//general constructor -- WIP
Game::Game(const stringVar &name, const int &maxPlayers, const stringVar &rules) : gameName(name), numPlayersMax(maxPlayers), numPlayers(0), numAudience(0), rulesOfTheGame(rules) {}

Game::Game(const stringVar &name) : gameName(name) {}

Game::startGame(const json *config)
{
	EngineImpl<json> engine{config};
	Game game = engine.initalizeEngine();
}

void Game::addPlayer(const Player &p)
{
	if (numPlayers != numPlayersMax)
	{
		allPlayers.push_back(p);
		numPlayers++;
	}
	else
	{
		std::cout << "Game currently at maximum capacity. Player (" << p.getPlayerID() << ") unable to join. \n";
	}
}

void Game::removePlayer(const variable &pid)
{
	//placeholder - looking for alternative solutions for organizing vector of players
	allPlayers.erase(std::find_if(allPlayers.begin(), allPlayers.end(), Player(pid, "name")));
	numPlayers--;
}

int Game::getNumPlayers()
{
	return numPlayers;
}
int Game::getNumPlayersMax()
{
	return numPlayersMax;
}

std::vector<Player> Game::getAllPlayers()
{
	return allPlayers;
}

std::vector<stringVar> getAllPlayersInputs() { return allPlayersInputs; }

//setAllPlayersInputs function will act as an alternative to game turns
//since some games won't require turns, like rock, paper, scissors
void Game::setAllPlayersInputs()
{
	//Unfinished
}

void Game::addAudience(const Audience &a)
{
	if (audience)
	{
		allAudience.push_back(a);
		numAudience++;
	}
	else
	{
		std::cout << "This game does not allow Audience";
	}
}

void Game::removeAudience(const variable &pid)
{
	allAudience.erase(std::find_if(allPlayers.begin(), allPlayers.end(), [](auto& audienceMember){
		return audienceMember.getAudienceID() == pid;});
	numAudience--;
}

std::vector<Audience> getAllAudience(){
	return allAudience;
}

bool operator==(const Game &game1, const Game &game2) { return game1->gameName == game2->gameName; }
/*
*  WIP
*/

// std::string Game::rightAnswerForQuestion(const std::vector<std::string*>& questions){
// 	return ""; // find best way to match question with answers
// }

// void addQuestions(std::string question, std::string answer){

// }
