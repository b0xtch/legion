#include <chrono>
#include <thread>
#include <exception>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ParsedMessage.h"
#include "GameServer.h"
#include "Server.h"
#include "Client.h"

const int CLIENT_RECEIVE_ATTEMPTS = 10;
const int CLIENT_UPDATE_ATTEMPTS = 5;
const int SERVER_RECEIVE_ATTEMPTS = 10;

std::string makeMessage(const std::string& command, const std::string& data) {
    return "{\"command\":\"" + command + "\",\"data\":\"" + data + "\"}";
}

void updateClient(networking::Client& client) {
    for (int i = 0; i < CLIENT_UPDATE_ATTEMPTS; i++) {
        client.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

std::string receiveToClient(networking::Client& client) {
    int attempts = 0;
    std::string recv;
    while (attempts < CLIENT_RECEIVE_ATTEMPTS && recv.empty()) {
        client.update();
        recv = client.receive();
        attempts++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (attempts >= CLIENT_RECEIVE_ATTEMPTS) {
        throw std::runtime_error{"Timeout. Client unable to receive after " + std::to_string(CLIENT_RECEIVE_ATTEMPTS) + " attempts."};
    }
    return recv;
}

std::string updateGameServer(GameServer& gameServer) {
    try {
        for (int attempts = 0; attempts < SERVER_RECEIVE_ATTEMPTS && gameServer.getKeepRunning(); attempts++) {
            gameServer.update();
            gameServer.receive();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (std::exception& e) {
        std::cerr << "An error occurred updating the server: " << e.what() << "\nTerminating" << std::endl;
        return "";
    }
}

TEST(GameServerTests, portAndKeepRunning) {
    GameServerConfig gsc{};
    GameServer gameServer{gsc, 42010};
    EXPECT_EQ(42010, gameServer.getPort());
    EXPECT_TRUE(gameServer.getKeepRunning());
    
    networking::Client client{"localhost", "42010"};
    client.send(makeMessage(PMConstants::TYPE_SERVER_STOP, ""));
    //updateClient(client);
    client.update();
    //updateGameServer(gameServer);
    gameServer.update();
    gameServer.receive();
    EXPECT_FALSE(gameServer.getKeepRunning());
}

TEST(GameServerTests, responseToRequestGames) {
    GameServerConfig gsc{"../tests/gameServer/", 10, 100}; // Assumes test is being run from <project-dir>/build/
    GameServer gameServer{gsc, 42020};
    
    networking::Client client{"localhost", "42020"};
    client.send(makeMessage(PMConstants::TYPE_LIST_GAMES, ""));
    updateClient(client);
    
    updateGameServer(gameServer);
    std::string message;
    try {
        message = receiveToClient(client);
    }
    catch (std::runtime_error& e) {
        FAIL() << e.what();
    }
    
    auto expected = makeMessage(PMConstants::TYPE_LIST_GAMES, "Kahoot 2\\nRock Paper Scissors\\nZen Game");
    EXPECT_EQ(expected, message);
}

TEST(GameServerTests, sendToClients) {
    GameServerConfig gsc{"", 10, 100}; // Assumes test is being run from <project-dir>/build/
    GameServer gameServer{gsc, 42030};
}