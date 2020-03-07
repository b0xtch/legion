#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameClient.h"
#include "json.hpp"

using json = nlohmann::json;

// Message Creation Tests

json inputToJson(const std::string& testInput) {
    std::string serverMessage = makeServerMessage(testInput);
    json testJson = json::parse(serverMessage);
    return testJson;
}

TEST(clientOutgoingTest, createSessionMessage) {
    json testJson = inputToJson("!createsession sessionname");
    EXPECT_EQ( "!createsession", testJson.at("command") );
    EXPECT_EQ( "sessionname", testJson.at("data") );
}

TEST(clientOutgoingTest, joinSessionMessage) {
    json testJson = inputToJson("!joinsession sessionname");
    EXPECT_EQ( "!joinsession", testJson.at("command") );
    EXPECT_EQ( "sessionname", testJson.at("data") );
}

TEST(clientOutgoingTest, leaveSessionMessage) {
    json testJson = inputToJson("!leavesession");
    EXPECT_EQ( "!leavesession", testJson.at("command") );
}

TEST(clientOutgoingTest, gameInputMessage) {
    json testJson = inputToJson("!gameinput gamename");
    EXPECT_EQ( "!gameinput", testJson.at("command") );
    EXPECT_EQ( "gamename", testJson.at("data") );
}

TEST(clientOutgoingTest, whisperMessage) {
    json testJson = inputToJson("!whisper username");
    EXPECT_EQ( "!whisper", testJson.at("command") );
    EXPECT_EQ( "username", testJson.at("data") );
}

TEST(clientOutgoingTest, requestGamesMessage) {
    json testJson = inputToJson("!requestgames");
    EXPECT_EQ( "!requestgames", testJson.at("command") );
}

TEST(clientOutgoingTest, chatMessageExplicit) {
    json testJson = inputToJson("!chat this is a chat message");
    EXPECT_EQ( "!chat", testJson.at("command") );
    EXPECT_EQ( "this is a chat message", testJson.at("data") );
}

TEST(clientOutgoingTest, chatMessageImplicit) {
    json testJson = inputToJson("this is a chat message");
    EXPECT_EQ( "!chat", testJson.at("command") );
    EXPECT_EQ( "this is a chat message", testJson.at("data") );
}

TEST(clientOutgoingTest, invalidCommandMessage) {
    json testJson = inputToJson("!wrong this is a chat message");
    EXPECT_EQ( "!chat", testJson.at("command") );
    EXPECT_EQ( "!wrong this is a chat message", testJson.at("data") );
}

TEST(clientOutgoingTest, invalidCommandMessageCapslock) {
    json testJson = inputToJson("!CREATESESSION this is a chat message");
    EXPECT_EQ( "!chat", testJson.at("command") );
    EXPECT_EQ( "!CREATESESSION this is a chat message", testJson.at("data") );
}

// Message Parsing Tests
// WIP
// Server response format not yet finalized
// processServerMessage not yet finished

TEST(clientIncomingTest, chatServerMessage) {
    std::string testResponse = "{ \"command\" \" !chat\", \"data\": \"this is a chat message\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "this is a chat message", processedTestResponse );
}

TEST(clientIncomingTest, sessionCreateSuccess) {
    std::string testResponse = "{ \"command\" \" !createsession\", \"data\": \"success\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "The session was successfully created.", processedTestResponse );
}

TEST(clientIncomingTest, sessionCreateFail) {
    std::string testResponse = "{ \"command\" \" !createsession\", \"data\": \"fail\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "There was an error creating the session.", processedTestResponse );
}

TEST(clientIncomingTest, sessionJoinSuccess) {
    std::string testResponse = "{ \"command\" \" !joinsession\", \"data\": \"sessionname\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "You have joined the session: sessionname", processedTestResponse );
}

TEST(clientIncomingTest, sessionJoinFail) {
    std::string testResponse = "{ \"command\" \" !joinsession\", \"data\": \"fail\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "There was an error joining the session.", processedTestResponse );
}

TEST(clientIncomingTest, sessionLeave) {
    std::string testResponse = "{ \"command\" \" !leavesession\", \"data\": \"sessionname\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "You have left the session: sessionname", processedTestResponse );
}

TEST(clientIncomingTest, gameResponse) {
    std::string testResponse = "{ \"command\" \" !gameinput\", \"data\": \"this is game data\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "this is game data", processedTestResponse );
}

TEST(clientIncomingTest, gameListRequest) {
    std::string testResponse = "{ \"command\" \" !gamesList\", \"data\": \"this is a list of games\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "Choose a game: this is a list of games", processedTestResponse );
}
