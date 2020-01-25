#include "client.h"

const std::string WEBPAGE_LOCATION = "./webpage.html";

// This should be passed as into the "httpMessage" parameter of the Nick's server.
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

// 
void send(std::string contents) {
    
}