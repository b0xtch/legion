#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "GameServer.h"
#include "Utils.h"

// This is the file that the server admin must run to start the server.

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <config-file-path>" << std::endl;
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
    
    std::string_view configText;
    GameServerConfig gsConfig{};
    try {
        configText = Utils::loadFile(argv[2]);
        gsConfig = {configText};
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    
    GameServer gameServer{gsConfig, port, ""};
    
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
        
        // https://stackoverflow.com/a/10613664
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    return 0;
}