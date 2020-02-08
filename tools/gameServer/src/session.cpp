#include "session.h"
#include <algorithm>
#include <stdexcept>
#include "networking.h"

namespace Session {

    /**
     * Construct with session id
     * **/
    Session(): generateSessionId{5}{}
    

    /**
     * This function returns all available connections
     * **/
    std::vector<networking::Connection> getAllClients(){
        return clients;
    };


    /**
     * This function returns particular connection if exists
     * **/
    networking::Connection getClient(const Connection& connection){
        auto found = clients.find(connection) != clients.end());
        if (found == clients.end()){
            throw ConnectionNotFound();
        };

        return *found;
    };

    
    /**
     * This function adds new connection to session given session limit has'nt reached yet
     * **/
    void addClient(const networking::Connection& connection){
         if(clients.size() >= MAX_SESSION_SIZE){
            throw SessSessionLimitExceeded();
         };
         
         clients.add(client);
    };
    

    /**
     * Simple getter for getting sesion id
     * **/
    std::string getSessionId(){
        return sessionId;
    };

    
    /**
     * Check if connection is part of this session
     * **/
    bool isClient(const networking::Connection& connection){
        return clients.find(connection) != clients.end();
    }
}

