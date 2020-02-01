#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include <unordered_set>

class Session {
public:
    Session();
    std::vector<networking::Connection> getAllClients();
    networking::Connection getClient(uintptr_t id);
    void addClient(networking::Connection connection);
    bool isClient(networking::Connection connection);
    void broadCastMessage(std::string message);
    std::string getSessionId();
private:
    std::unordered_set<networking::Connection> clients;
    std::string sessionId;
    int sessionClientLimit;
}

#endif
