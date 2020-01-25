#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <string>
#include <iostream>
#include <fstream>

// This should be passed as into the "httpMessage" parameter of Nick's server class.
std::string getHtmlForNewContents();

class GameClient {
public:
  GameClient();
  ~GameClient();

  void joinSession(std::string sessionId);
  void createSession();
  void sendData(Data& data); // Will create a custom data structure that holds data 
  void receiveData();
}

#endif
