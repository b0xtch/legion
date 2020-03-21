#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <string>
#include <map>

#include "SessionManager.h"
#include "Server.h"

/** Handles the loading and parsing of the server configuation file only. */
class GameServerConfig {
public:
    GameServerConfig();
    GameServerConfig(const std::string& gameDir, int maxSessions, int maxConnections);
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
    /** Create a game server with the given configuration at a given port. */
    GameServer(GameServerConfig gameServerConfig, unsigned short port);
    
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
    
private:
    /** Create a message representing the list of games that can be played. */
    networking::Message generateGameListResponse(networking::Connection recipient);
    
    /** Fills a structure that maps a game name to a game file. */
    void fillGameFilesMap();
    
    bool keepRunning;
    unsigned short port;
    networking::Server server;
    SessionManager sessionManager;
    GameServerConfig gameServerConfig;
    std::map<std::string, std::string> gameNameToPathMap;
};

#endif
