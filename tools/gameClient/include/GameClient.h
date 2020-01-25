#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

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
