#include "GameClient.h"

// This will define the location of the webpage that will be sent
// to players that connected to the server for the first time

void GameClient::createSession(){
  std::string sessionId = Utils::generateSessionId();
};

void GameClient::joinSession(std::string sessionId){
};

void GameClient::sendData(Data& data) {
  
}

void GameClient::receiveData() {
  
}

const std::string WEBPAGE_LOCATION = "./webpage.html";

std::string getHtmlForNewConnections() {
  std::ifstream htmlFileStream;
  htmlFileStream.open(WEBPAGE_LOCATION);
  if (htmlFileStream.fail()) {
    std::cerr << "ERROR: Unable to open webpage file at \"\n"
      << WEBPAGE_LOCATION << "\".";
  }
  else {
    std::string htmlFileContents = std::string{
      std::istreambuf_iterator<char>(htmlFileStream), 
        std::istreambuf_iterator<char>()};
    htmlFileStream.close();
    return htmlFileContents;
  }
}
