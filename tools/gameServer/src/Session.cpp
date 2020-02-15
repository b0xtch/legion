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
std::unordered_map<ConnectionId, Connection>& Session::getAllClients(){
    return clients;
};


/**
 * This function returns particular connection if exists
 * **/
Connection Session::getClient(const Connection& connection){
    if (isClient(connection)){
        return clients[connection.id];
    }
    throw;
};


/**
 * This function adds new connection to session given session limit has'nt reached yet
 * **/
void Session::addClient(const Connection& connection){
    if(clients.size() >= MAX_SESSION_SIZE){
        throw;// SessSessionLimitExceeded();
    };

    clients[connection.id] = connection;
};


/**
 * Simple getter for getting sesion id
 * **/
std::string Session::getSessionId(){
    return sessionId;
};


/**
 * Check if connection is part of this session
 * **/
bool Session::isClient(const Connection& connection){
    return clients.find(connection.id) != clients.end();
}
