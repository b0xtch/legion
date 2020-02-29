#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameClient.h"
#include "json.hpp"

using json = nlohmann::json;

// WIP
// To do: add tests for client/server communication and server responses

// Message Creation Tests

json inputToJson(const std::string& testInput) {
    std::string serverMessage = makeServerMessage(testInput);
    json testJson = json::parse(serverMessage);
    return testJson;
}

TEST(GameClientTests, createSessionMessage) {
    json testJson = inputToJson("!createsession sessionname");
    EXPECT_EQ( "!createsession", testJson.at("command") );
    EXPECT_EQ( "sessionname", testJson.at("data") );
}

TEST(GameClientTests, joinSessionMessage) {
    json testJson = inputToJson("!joinsession sessionname");
    EXPECT_EQ( "!joinsession", testJson.at("command") );
    EXPECT_EQ( "sessionname", testJson.at("data") );
}

TEST(GameClientTests, leaveSessionMessage) {
    json testJson = inputToJson("!leavesession");
    EXPECT_EQ( "!leavesession", testJson.at("command") );
}

TEST(GameClientTests, gameInputMessage) {
    json testJson = inputToJson("!gameinput gamename");
    EXPECT_EQ( "!gameinput", testJson.at("command") );
    EXPECT_EQ( "gamename", testJson.at("data") );
}

TEST(GameClientTests, whisperMessage) {
    json testJson = inputToJson("!whisper username");
    EXPECT_EQ( "!whisper", testJson.at("command") );
    EXPECT_EQ( "username", testJson.at("data") );
}

TEST(GameClientTests, requestGamesMessage) {
    json testJson = inputToJson("!requestgames");
    EXPECT_EQ( "!requestgames", testJson.at("command") );
}

TEST(GameClientTests, chatMessageExplicit) {
    json testJson = inputToJson("!chat this is a chat message");
    EXPECT_EQ( "!chat", testJson.at("command") );
    EXPECT_EQ( "this is a chat message", testJson.at("data") );
}

TEST(GameClientTests, chatMessageImplicit) {
    json testJson = inputToJson("this is a chat message");
    EXPECT_EQ( "!chat", testJson.at("command") );
    EXPECT_EQ( "this is a chat message", testJson.at("data") );
}

TEST(GameClientTests, invalidCommandMessage) {
    json testJson = inputToJson("!wrong this is a chat message");
    EXPECT_EQ( "!chat", testJson.at("command") );
    EXPECT_EQ( "!wrong this is a chat message", testJson.at("data") );
}

TEST(GameClientTests, invalidCommandMessageCapslock) {
    json testJson = inputToJson("!CREATESESSION this is a chat message");
    EXPECT_EQ( "!chat", testJson.at("command") );
    EXPECT_EQ( "!CREATESESSION this is a chat message", testJson.at("data") );
}

// Message Parsing Tests
// WIP
// Server response format not yet finalized
// processServerMessage not yet finished

TEST(GameClientTests, chatServerMessage) {
    std::string testResponse = "{ \"command\" \" !chat\", \"data\": \"this is a chat message\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "this is a chat message", processedTestResponse );
}

TEST(GameClientTests, gameOutputServerMessage) {
    std::string testResponse = "{ \"command\" \" !gameOutput\", \"data\": \"this is game output\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "game output here", processedTestResponse );
}

TEST(GameClientTests, gamesListServerMessage) {
    std::string testResponse = "{ \"command\" \" !gamesList\", \"data\": \"this is a list of games\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "games list here", processedTestResponse );
}
