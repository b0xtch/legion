#ifndef GAME_H_
#define GAME_H_
#include <string>
class Game{
public:
  Game();
  ~Game();
  string rightAnswerForQuestion(const std::vector<string*>& questions);
private:
  int numPlayers;
  int numPlayersMax;
  int numQuestions;
  //int playerPoints;
  string gameName;
  string rightAnswer;
  string rulesOfTheGame;
  //string* listOfAnswers;
  std::vector<string*> questions;
};

Game(){
  gameName = "Jackbox no TV";
  numPlayersMax = 8;
  numPlayers = 0; //setting to 0 so that the client server connection can determine number of players connecting
  rulesOfTheGame = "Welcome to Jackbox no TV, the rules of the game are ...";
}

~Game(){
  for(string* question: questions){ delete[] question; }
  questions.clear();
  std::cout<<"Deleted vector questions, size of vector = "<<questions.size()<<endl;
}


#endif
