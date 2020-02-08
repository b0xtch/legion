#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <string>

#include "Server.h"
#include "SessionManager.h"

class GameServer {
public:
    GameServer(int port, std::string htmlFile);
    
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
    
    enum MessageType {
        Other, ServerStop, CreateSession, JoinSession
    };
    static MessageType parseMessageType(std::string text);
};

#endif