#include <chrono>
#include <thread>
#include <exception>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ParsedMessage.h"
#include "GameServer.h"
#include "Server.h"
#include "Client.h"

const int CLIENT_RECEIVE_ATTEMPTS = 3;
const int CLIENT_UPDATE_ATTEMPTS = 3;
const int SERVER_UPDATE_ATTEMPTS = 3;
const int WARMUP_ROUNDS = 3;

std::string makeMessage();
void sendFromClient(networking::Client& client, const std::string& message);
void updateClient(networking::Client& client);
std::string receiveToClient(networking::Client& client);
void updateGameServer(GameServer& gameServer);
void warmup(networking::Client& client, GameServer& gameServer);

std::string makeMessage(const std::string& command, const std::string& data) {
    return "{\"command\":\"" + command + "\",\"data\":\"" + data + "\"}";
}

void sendFromClient(networking::Client& client, const std::string& message) {
    client.send(message);
    updateClient(client);
}

void updateClient(networking::Client& client) {
    for (int i = 0; i < CLIENT_UPDATE_ATTEMPTS; i++) {
        client.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

std::string receiveToClient(networking::Client& client) {
    int attempts = 0;
    std::string recv;
    while (attempts < CLIENT_RECEIVE_ATTEMPTS && recv.empty()) {
        client.update();
        recv = client.receive();
        attempts++;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    if (attempts >= CLIENT_RECEIVE_ATTEMPTS) {
        throw std::runtime_error{"Timeout. Client unable to receive after " + std::to_string(CLIENT_RECEIVE_ATTEMPTS) + " attempts."};
    }
    return recv;
}

void updateGameServer(GameServer& gameServer) {
    try {
        for (int attempts = 0; attempts < SERVER_UPDATE_ATTEMPTS && gameServer.getKeepRunning(); attempts++) {
            gameServer.update();
            gameServer.receive();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    catch (std::exception& e) {
        std::cerr << "An error occurred updating the server: " << e.what() << std::endl;
    }
}

void warmup(networking::Client& client, GameServer& gameServer) {
    client.update();
    for (int i = 0; i < WARMUP_ROUNDS; i++) {
        gameServer.update();
        gameServer.receive();
        client.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

TEST(GameServerTests, portAndKeepRunning) {
    GameServerConfig gsc{};
    GameServer gameServer{gsc, 42010};
    EXPECT_EQ(42010, gameServer.getPort());
    EXPECT_TRUE(gameServer.getKeepRunning());
    
    networking::Client client{"localhost", "42010"};
    
    warmup(client, gameServer);
    sendFromClient(client, makeMessage(PMConstants::TYPE_SERVER_STOP, ""));
    updateGameServer(gameServer);
    
    EXPECT_FALSE(gameServer.getKeepRunning());
}

TEST(GameServerTests, responseToRequestGames) {
    GameServerConfig gsc{"../tests/testFiles/gameServer/", 10, 100}; // Assumes test is being run from <project-dir>/build/
    GameServer gameServer{gsc, 42020};
    networking::Client client{"localhost", "42020"};
    
    warmup(client, gameServer);
    sendFromClient(client, makeMessage(PMConstants::TYPE_LIST_GAMES, ""));
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

TEST(IgnoreGameServerTests, sendToClients) {
    GameServerConfig gsc{"", 10, 100};
    GameServer gameServer{gsc, 42030};
}