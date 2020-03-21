#include <chrono>
#include <thread>
#include <exception>
#include <memory>

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

/** Tests the port it was constructed with. */
TEST(GameServerTests, port) {
    GameServerConfig gsc{};
    GameServer gameServer{gsc, 42010};
    EXPECT_EQ(42010, gameServer.getPort());
}

/** Tests if the GameServer sets a flag to stop running depending on the message. */
TEST(GameServerTests, keepRunning) {
    GameServerConfig gsc{"", 10, 100};
    GameServer gameServer{gsc, 42020};
    EXPECT_TRUE(gameServer.getKeepRunning());
    
    networking::Client client{"localhost", "42020"};
    
    warmup(client, gameServer);
    sendFromClient(client, makeMessage(PMConstants::TYPE_SERVER_STOP, ""));
    updateGameServer(gameServer);
    
    EXPECT_FALSE(gameServer.getKeepRunning());
}

/** Tests if the GameServer can respond to clients that ask for list of games. */
TEST(GameServerTests, responseToRequestGames) {
    GameServerConfig gsc{"../tests/testfiles/gameServer/", 10, 100}; // Assumes test is being run from <project-dir>/build/
    GameServer gameServer{gsc, 42030};
    
    networking::Client client{"localhost", "42030"};
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
    
    networking::Client client2{"localhost", "42030"};
    warmup(client2, gameServer);
    sendFromClient(client2, makeMessage(PMConstants::TYPE_LIST_GAMES, ""));
    updateGameServer(gameServer);
    try {
        message = receiveToClient(client2);
    }
    catch (std::runtime_error& e) {
        FAIL() << e.what();
    }
    expected = makeMessage(PMConstants::TYPE_LIST_GAMES, "Kahoot 2\\nRock Paper Scissors\\nZen Game");
    EXPECT_EQ(expected, message);
}

/** Tests if the GameServer will cause clients to disconnect when it goes out of scope. */
TEST(GameServerTests, disconnection) {
    GameServerConfig gsc{}; // Assumes test is being run from <project-dir>/build/
    
    std::unique_ptr<GameServer> gameServerPtr = std::make_unique<GameServer>(gsc, 42040);
    
    networking::Client client1{"localhost", "42040"};
    networking::Client client2{"localhost", "42040"};
    warmup(client1, *gameServerPtr);
    warmup(client2, *gameServerPtr);
    
    gameServerPtr.reset(nullptr); // Cause gameServer's destructor to be called
    
    updateClient(client1);
    updateClient(client2);
    
    EXPECT_TRUE(client1.isDisconnected());
    EXPECT_TRUE(client2.isDisconnected());
}