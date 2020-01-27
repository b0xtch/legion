#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <string>
#include <iostream>
#include <fstream>

#include "Client.h"
#include "Util.h"
#include "players.h"
#include "server.h"
#include "Data.h"

// This should be passed as into the "httpMessage" parameter of Nick's server class.
std::string getHtmlForNewContents();

// This class represents a single player connected to the server.
class GameClient {
private:
  Players player;
  networking::Client client;
public:
  GameClient(std::string_view address, std::string_view port);
  ~GameClient();

  void joinSession(std::string sessionId);
  void createSession();
  void sendData(Data& data); // Will create a custom data structure that holds data 
  Data receiveData(std::string content); // This is registered as a callback function to be called when data is received
  
  Players getPlayer() const;
};

#endif
