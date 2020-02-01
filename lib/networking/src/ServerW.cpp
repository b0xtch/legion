void update() {
    parentServer.update();
}

void sendTo(std::vector<Connection> connections, std::string text) {

    std::deque<Message> newMessages;
    for (auto client : connections) {
        newMessages.push_back({client, text});
    }
    parentServer.send(newMessages);

}

void disconnect(Connection connection) {
    parentServer.disconnect(connection);
}
