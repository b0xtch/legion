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
void Session::addUser(const User& user){
    if(users.size() >= MAX_SESSION_SIZE){
        throw;// SessSessionLimitExceeded();
    };
    users.insert(user);
};


/**
 * Simple getter for getting sesion id
 * **/
std::string Session::getSessionId(){
    return sessionId.id;
};


/**
 * Check if connection is part of this session
 * **/
bool Session::isUser(const User& user){
    return users.find(user) != users.end();
}


/**
 * Method for removing connection from session
 * **/
void Session::removeUser(const User &user){
    if(users.find(user) != users.end()){
        users.erase(user);
    }
}

