#include "GameClient.h"

const size_t SESSION_ID_LENGTH = 8;

void GameClient::Client(std::string_view address, std::string_view port)
  : client{address, port}
{
}

void GameClient::createSession(){
  string sessionId = Utils::generateSessionId(SESSION_ID_LENGTH);
}

void GameClient::joinSession(std::string sessionId){
}

void GameClient::sendData(Data& data) {
  
}

Players GameClient::getPlayers() const {
  return player;
}

Data GameClient::receiveData(std::string content) {
  return {content} // Parses the content received in the message into Data
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