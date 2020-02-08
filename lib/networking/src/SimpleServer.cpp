void SimpleServer::update() {
    parentServer.update();
}

void SimpleServer::send(const std::deque<Message>& messages) {
    parentServer.send(messages);
}

void SimpleServer::queueMessage(const std::vector<Connection>& connections, std::string text) {

    for (auto client : connections) {
        outgoingMessages.push_back({client, text});
    }

}

void SimpleServer::sendQueuedMessages() {
    parentServer.send(messageQueue);
}

std::deque<Message> SimpleServer::receive() {
    return parentServer.receive();
}

std::string getReceivedText() {
    std::string messageText;
    std::deque<Message> incomingMessages = parentServer.receive();

    for (const &message : incomingMessages) {
        messageText += message.text;
        messageText += '\n';
    }

    return messageText;
}

void SimpleServer::disconnect(Connection connection) {
    parentServer.disconnect(connection);
}

