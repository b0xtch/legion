#ifndef GAME_H_
#define GAME_H_
#include <string>
#include <vector>
// #include "session.h"
#include "players.h"

// class Session;
class Game{
  // friend class Session;
public:
  Game(); //defauult for Jackbox no TV
  Game(std::string name, int maxPlayers, std::string rules); // general constructor to support additional games, will add more parameters
  ~Game();
  void addPlayer(Players p);
  void removePlayer(int pid); //remove using playerID
  // std::string rightAnswerForQuestion(const std::vector<std::string*>& questions);
  void addQuestions(std::string question, std::string answer);
  //rightAnswerForQuestion and addQuestions will be moved to a separate child class inherited from Game class

  int getNumPlayers(); //interpreted from json config
  int getNumPlayersMax(); //interpreted from json config

  std::vector<Player> getAllPlayers();
  virtual void displayInfo(Game& displayGame);

private:
  int numPlayers; //interpreted from json config
  int numPlayersMax; //interpreted from json config
  int numQuestions;
  //int playerPoints;
  std::string gameName; //interpreted from json config
  // std::string rightAnswer; //is this still needed?
  std::string rulesOfTheGame; //interpreted from json config, might be made into an object???
  std::string* listOfAnswers;
  std::vector<std::string*> questions;
  std::vector<Player> allPlayers;
  boolean audience; //interpreted from json config
};


#endif
