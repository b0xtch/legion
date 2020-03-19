#include <chrono>
#include <thread>
#include <exception>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ParsedMessage.h"
#include "GameServer.h"
#include "Server.h"
#include "Client.h"

const int CLIENT_RECEIVE_ATTEMPTS = 50;
const int SERVER_RECEIVE_ATTEMPTS = 50;

std::string makeMessageString(const std::string& command, const std::string& data) {
    return "{\"command\":\"" + command + "\", \"data\":\"" + data + "\"}";
}

std::string receiveToClient(networking::Client& recipient) {
    int attempts = 0;
    std::string recv;
    while (attempts < CLIENT_RECEIVE_ATTEMPTS && recv.empty()) {
        recipient.update();
        recv = recipient.receive();
        attempts++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (attempts >= CLIENT_RECEIVE_ATTEMPTS) {
        throw std::runtime_error{"Timeout. Client unable to receive after " + std::to_string(CLIENT_RECEIVE_ATTEMPTS) + " attempts."};
    }
    return recv;
}

TEST(GameServerTests, portAndKeepRunning) {
    GameServerConfig gsc{};
    GameServer gameServer{gsc, 42069};
    EXPECT_EQ(42069, gameServer.getPort());
    EXPECT_TRUE(gameServer.getKeepRunning());
    
    networking::Client client{"localhost", "42069"};
    client.send(makeMessageString(PMConstants::TYPE_SERVER_STOP, ""));
    
    int attempts = 0;
    while (attempts < SERVER_RECEIVE_ATTEMPTS && gameServer.getKeepRunning()) {
        gameServer.update();
        gameServer.receive();
        attempts++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    EXPECT_FALSE(gameServer.getKeepRunning());
}

TEST(GameServerTests, responseToRequestGames) {
    
}

TEST(GameServerTests, responseOthers) {
    
}