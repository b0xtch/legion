#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <string>

#include "SessionManager.h"
#include "Server.h"

/** Handles the loading and parsing of the server configuation file only. */
class GameServerConfig {
public:
    GameServerConfig();
    void parse(const std::string& configData);
    
    std::string getGameConfigDir() const;
    int getMaxSessions() const;
    int getMaxConnections() const;
private:
    std::string gameDir;
    int maxSessions;
    int maxConnections;
    
    const std::string CFGKEY_GAME_DIR = "games";
    const std::string CFGKEY_MAX_SESSIONS = "maxSessions";
    const std::string CFGKEY_MAX_CONNECTIONS = "maxConnections";
};

class GameServer {
public:

    GameServer(GameServerConfig gameServerConfig, unsigned short port, const std::string& htmlFile);
    
    /** Useful for testing the GameServer. Perfroms an std::move on the server. */
    GameServer(GameServerConfig gameServerConfig, networking::Server& server, SessionManager& sessionManager);
    
    /** Sends out all the messages passed to the intended clients. */
    void send(const std::deque<networking::Message>& messages);
    
    /** Allows the server to queue new incoming messages to receive them. */
    void update();
    
    /** Allows the server to update games, sessions, and basically work. */
    void receive();
    
    /** Returns the port that the server was initialized with. */
    unsigned short getPort() const;
    
    /** Returns if the server should keep running. */
    bool getKeepRunning() const;
    
    /** Returns the location of the HTML file that it was initialized with. */
    std::string_view getHtmlFile() const;
    
private:
    networking::Message generateGameListResponse(networking::Connection recipient);
    
    bool keepRunning;
    unsigned short port;
    std::string_view htmlFile;
    networking::Server server;
    SessionManager sessionManager;
    GameServerConfig gameServerConfig;
};

#endif
