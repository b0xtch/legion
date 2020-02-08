#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <string>

#include "SimpleServer.h"
#include "SessionManager.h"

class GameServer {
public:
    GameServer(int port, std::string htmlFile);
    
    /** Sends out all the messages passed to the intended clients. */
    void send(const std::deque<networking::Message>& messages);
    /** Sends out text to the given clients. */
    void sendTextTo(const std::vector<Connection>& connections, std::string text);
    
    /** Allows the server to queue new incoming messages to receive them. */
    void update();
    
    /** Allows the server to update games, sessions, and basically work. */
    std::string receive();
    
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
    SimpleServer server;
    SessionManager sessionManager;
    std::vector<Connection> clients;
    
    enum MessageType {
        Other, ServerStop, CreateSession, JoinSession, LeaveServer
    };
    static MessageType parseMessageType(std::string text);
};

#endif
