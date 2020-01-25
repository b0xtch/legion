#include "GameClient.h"

const size_t SESSION_ID_LENGTH = 8;

void GameClient::Client(std::string_view address, std::string_view port){
  networking::Client(address, port);
};

void GameClient::createSession(){
  string sessionId = Utils::generateSessionId(SESSION_ID_LENGTH);
};

void GameClient::joinSession(std::string sessionId){
};

