#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <string>

#include "SessionManager.h"
#include "Server.h"

/** Handles the loading and parsing of the server configuation file only. */
class GameServerConfig {
public:
    GameServerConfig();
    GameServerConfig(const std::string& configLocation);
    
    std::string_view getGameConfigDir() const;
    
private:
    std::string_view configLocation;
    std::string_view gameDir;
    
    const std::string CFGKEY_GAME_DIR = "games";
}

class GameServer {
public:

    GameServer(GameServerConfig gameServerConfig, int port, const std::string& htmlFile);
    
    /** Useful for testing the GameServer. Perfroms an std::move on the server. */
    GameServer(GameServerConfig gameServerConfig, networking::Server& server, SessionManager& sessionManager);
    
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
    GameServerConfig gameServerConfig;
};

#endif
