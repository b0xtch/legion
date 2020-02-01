#ifndef NETWORKING_SERVERW_H
#define NETWORKING_SERVERW_H

#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

Class ServerW : public Server {

public:
    ServerW (Server* server) : parentServer(server) { }

    void update();
    void sendTo(const std::deque<Message>& messages); // Send to chosen client
    void sendToAll(const std::deque<Message>& messages); // Send to all clients
    void disconnect(Connection connection);

private:
    Server parentServer;

};


