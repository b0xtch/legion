#include <iostream>
#include <string>
#include <unistd.h>

#include "GameServer.h"

// This is the file that the server admin must run to start the server.

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>";
        return 1;
    }
    
    unsigned short port = std::stoi(argv[1]);
    GameServer gameServer{port};
    
    bool keepRunning = true;
    while (keepRunning) {
        try {
            gameServer.update();
        }
        catch (std::exception& e) {
            std::cerr << "An error occurred updating the server: " << e.what() << "\nTerminating" << std::endl;
            keepRunning = false;
        }
    }
    
    return 0;
}