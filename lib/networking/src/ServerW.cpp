void ServerW::update() {
    parentServer.update();
}

void ServerW::send(const std::deque<Message>& messages) {
    parentServer.send(messages);
}

void ServerW::queueMessage(std::vector<Connection> connections, std::string text) {

    for (auto client : connections) {
        messageQueue.push_back({client, text});
    }

}

void ServerW::sendQueuedMessages() {
    parentServer.send(messageQueue);
}

std::deque<Message> ServerW::receive() {
    return parentServer.receive;
}

void ServerW::disconnect(Connection connection) {
    parentServer.disconnect(connection);
}
