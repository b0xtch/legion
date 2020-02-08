void SimpleServer::update() {
    parentServer.update();
}

void SimpleServer::send(const std::deque<Message>& messages) {
    parentServer.send(messages);
}

void SimpleServer::queueMessage(std::vector<Connection> connections, std::string text) {

    for (auto client : connections) {
        outgoingMessages.push_back({client, text});
    }

}

void SimpleServer::sendQueuedMessages() {
    parentServer.send(messageQueue);
}

std::deque<Message> SimpleServer::receive() {
    return parentServer.receive;
}

void SimpleServer::disconnect(Connection connection) {
    parentServer.disconnect(connection);
}

