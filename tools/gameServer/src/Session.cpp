#include "Session.h"
#include <algorithm>
#include <stdexcept>
#include "Utils.h"

/**
 * Construct with session id
 * **/
Session::Session(): sessionId{Utils::generateSessionId(6)}{};


/**
 * This function returns all available connections
 * **/
std::set<User> Session::getAllUsers(){
    return users;
};


/**
 * This function adds new connection to session given session limit has'nt reached yet
 * **/
void Session::addUser(const Connection& connection){
    if(users.size() >= MAX_SESSION_SIZE){
        throw;// SessSessionLimitExceeded();
    };
    users.insert(User{connection});
};


/**
 * Simple getter for getting sesion id
 * **/
std::string Session::getSessionId() const{
    return sessionId.id;
};


/**
 * Check if connection is part of this session
 * **/
bool Session::isUser(const Connection &connection) const{
    auto it = find_if(users.begin(), users.end(), [&](const User &user){return user.getConnection() == connection;});
    return it != users.end();
}


/**
 * Method for removing connection from session
 * **/
void Session::removeUser(const Connection &connection){
    auto it = find_if(users.begin(), users.end(), [&](const User &user){return user.getConnection() == connection;});
    if(it != users.end()){
        users.erase(it);
    }
}

bool Session::operator< (const Session &session) const{
    return this->getSessionId() >=  session.getSessionId();
}

