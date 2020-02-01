#include "session.h"
#include <algorithms>
#include <stdexcept>

namespace Session{
    
    std::vector<networking::Connection> getAllClients(){
        return clients;
    };

    networking::Connection getClient(uintptr_t id){
        auto it = find_if(
            clients.begin(), clients.end(), [=](const networking::Connection &connection){
                return connection.id == id;
            }
        );

        if (it == clients.end()){
            throw std::invalid_argument("No client exists for this id");
        };

        return *it;
    };
    
    void addClient(networking::Connection connection){
         if(clients.size() >= sessionClientLimit){
            throw std::invalid_argument("Session Limit Exceeded");
         };
         
         clients.push_back(client);
    };
    
    std::string getSessionId(){
        return sessionId;
    };

    bool isClient(networking::Connection connection){
        return clients.find(connection) != connection.end();
      );
    }
}

