#ifndef SESSION_H
#define SESSION_H

#include <unordered_set>
#include "Server.h"

using networking::Server;
using networking::Connection;
using networking::Message;
using ConnectionId = uintptr_t;

/**
 * This class is responsible for handle communication between single game/session
 **/
class Session {

public:
    
    Session();
    
    std::unordered_map<ConnectionId, Connection>& getAllClients();
    
    Connection getClient(const Connection &connection);
    
    void addClient(const Connection &connection);
    
    bool isClient(const Connection &connection);
    
    std::string getSessionId();

private:
    
    std::unordered_map<ConnectionId, Connection> clients;
    
    std::string sessionId;
    
    int MAX_SESSION_SIZE;
};

#endif
