#ifndef NETWORKING_SERVERW_H
#define NETWORKING_SERVERW_H

#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "server.h"

/**
A wrapper class for Server, with functions intended to help with communicating with several clients in different session.
**/

Class ServerW : public Server {

public:
    ServerW (Server* server) : parentServer(server) { }

    void update();
    void send(const std::deque<Message>& messages);
    void sendTo(std::vector<Connection> connections, std::string text); // Send the same message to a list of clients
    [[nodiscard]] std::deque<Message> receive();
    void disconnect(Connection connection);

private:
    Server parentServer;

};


