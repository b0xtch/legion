#ifndef SESSION_H
#define SESSION_H


class Session {
public:
    Session();
    std::vector<Client> getAllClients();
    Client getClient();
    void addClient(Client client);
    std::string getSessionId();
private:
    std::vector<Clients> clients;
    std::string sessionId;
    int sessionClientLimit;
}

#endif
