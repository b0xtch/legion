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
    
    unsigned short port;
    // Error handling for the port.
    try {
        int rangeCheck = std::stoi(argv[1]);
        if (rangeCheck < 1 || rangeCheck > 65535) {
            std::cerr << "Error: The port is out of range." << std::endl;
            return 1;
        }
        port = rangeCheck;
    }
    catch (std::invalid_argument& e) {
        std::cerr << "Error: The port is invalid." << std::endl;
        return 1;
    }
    catch (std::out_of_range& e) {
        std::cerr << "Error: The port is out of range." << std::endl;
        return 1;
    }
    
    GameServer gameServer{port, ""};
    
    bool keepRunning = true;
    while (keepRunning) {
        try {
            gameServer.update();
        }
        catch (std::exception& e) {
            std::cerr << "An error occurred updating the server: " << e.what() << "\nTerminating" << std::endl;
            keepRunning = false;
        }
        
        gameServer.receive();
        keepRunning = gameServer.getKeepRunning();
    }
    
    return 0;
}