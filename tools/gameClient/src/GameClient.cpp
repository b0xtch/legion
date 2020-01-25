#include "GameClient.h"

// This will define the location of the webpage that will be sent
// to players that connected to the server for the first time
const std::string WEBPAGE_LOCATION = "./webpage.html";

void GameClient::createSession(){
  std::string sessionId = Utils::generateSessionId();
};

void GameClient::joinSession(std::string sessionId){
};

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