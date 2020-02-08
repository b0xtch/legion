#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <string>

#include "Server.h"
#include "SessionManager.h"

/** Handles the loading and parsing of the server configuation file only. */
class GameServerConfig {
public:
    GameServerConfig(const std::string& configLocation);
    
    std::string_view getGameConfigDir() const;
    
private:
    std::string_view configLocation;
    
    std::string_view gameConfigDir;
}

class GameServer {
public:
    GameServer(int port, const std::string& htmlFile);
    
    /** Useful for testing the GameServer. Perfroms an std::move on the server. */
    GameServer(networking::Server& server, SessionManager& sessionManager);
    
    /** Sends out all the messages passed to the intended clients. */
    void send(const std::deque<networking::Message>& messages);
    
    /** Allows the server to queue new incoming messages to receive them. */
    void update();
    
    /** Allows the server to update games, sessions, and basically work. */
    void receive();
    
    /** Returns the port that the server was initialized with. */
    int getPort() const;
    
    /** Returns if the server should keep running. */
    bool getKeepRunning() const;
    
    /** Returns the location of the HTML file that it was initialized with. */
    std::string_view getHtmlFile() const;
    
private:
    bool keepRunning;
    int port;
    std::string_view htmlFile;
    networking::Server server;
    SessionManager sessionManager;
};

#endif