#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include <unordered_set>


/**
 * This class is responsible for handle communication between single game/session
 **/
class Session {

public:
    
    Session();
    
    std::vector<networking::Connection> getAllClients();
    
    networking::Connection getClient(const networking::Connection &connection);
    
    void addClient(const networking::Connection &connection);
    
    bool isClient(const networking::Connection &connection);
    
    std::string getSessionId();

private:
    
    std::unordered_set<networking::Connection> clients;
    
    std::string sessionId;
    
    int MAX_SESSION_SIZE;
}

#endif
