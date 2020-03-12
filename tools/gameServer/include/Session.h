#ifndef SESSION_H
#define SESSION_H

#include <set>
#include "Server.h"
#include "User.h"

using networking::Server;
using networking::Connection;
using networking::Message;
using ConnectionId = uintptr_t;


/**
 * A custom struct used for session id 
 * **/
struct SessionId{
    std::string id;
};


/**
 * This class is responsible for handle communication between single game/session
 **/
class Session {

public:
    
    Session();
    
    std::set<User> getAllUsers();
    
    Connection getClient(const Connection &connection);
    
    void addUser(const User &user);
    
    bool isUser(const User &user);

    void removeUser(const User &user);
    
    std::string getSessionId();

private:
    
    std::set<User> users;
    
    SessionId sessionId;
    
    int MAX_SESSION_SIZE;
};

#endif
