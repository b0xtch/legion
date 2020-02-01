void ServerW::update() {
    parentServer.update();
}

void ServerW::send(const std::deque<Message>& messages) {
    parentServer.send(messages);
}

void ServerW::sendTo(std::vector<Connection> connections, std::string text) {

    std::deque<Message> newMessages;
    for (auto client : connections) {
        newMessages.push_back({client, text});
    }
    parentServer.send(newMessages);

}

std::deque<Message> ServerW::receive() {
    return parentServer.receive;
}

void ServerW::disconnect(Connection connection) {
    parentServer.disconnect(connection);
}
