#ifndef GAME_H_
#define GAME_H_
#include <string>
#include <vector>
#include "players.h"

class Game{
public:
  Game(); //defauult for Jackbox no TV
  Game(std::string name, int maxPlayers, std::string rules); // general constructor to support additional games, will add more parameters
  ~Game();
  void addPlayer(Players p);
  void removePlayer(int pid); //remove using playerID
  std::string rightAnswerForQuestion(const std::vector<std::string*>& questions);
  void addQuestions(std::string question, std::string answer);

  int getNumPlayers();
  int getNumPlayersMax();

private:
  int numPlayers;
  int numPlayersMax;
  int numQuestions;
  //int playerPoints;
  std::string gameName;
  // std::string rightAnswer; //is this still needed?
  std::string rulesOfTheGame;
  //std::string* listOfAnswers;
  std::vector<std::string*> questions;
  std::vector<Players> allPlayers;
};


#endif
