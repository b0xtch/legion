#include "session.h"
#include <algorithms>
#include <stdexcept>

namespace Session{
    
    std::vector<Clients> getAllClients(){
        return clients;
    };

    Client getClient(clientId){
        auto it = find_if(
            clients.begin(), clients.end(), [=](Client client){
                return client.getId() == clientId;
            }
        );

        if (it == clients.end()){
            throw std::invalid_argument("No client exists for this id");
        };

        return *it;
    };
    
    void addClient(Client client){
         if(clients.size() >= sessionClientLimit){
            throw std::invalid_argument("Session Limit Exceeded");
         };
         
         clients.push_back(client);
    };
    
    std::string getSessionId(){
        return sessionId;
    };
}

