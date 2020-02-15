#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <string>

#include "SessionManager.h"
#include "Server.h"

/** Handles the loading and parsing of the server configuation file only. */
class GameServerConfig {
public:
    GameServerConfig(std::string_view configData);
    
    std::string_view getGameConfigDir() const;
    int getMaxSessions() const;
    int getMaxConnections() const;
private:
    std::string gameDir;
    int maxSessions = 10;
    int maxConnections = 50;
    
    const std::string CFGKEY_GAME_DIR = "games";
    const std::string CFGKEY_MAX_SESSIONS = "maxSessions";
    const std::string CFGKEY_MAX_CONNECTIONS = "maxConnections";
};

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
