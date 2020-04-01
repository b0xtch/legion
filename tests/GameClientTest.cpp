#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameClient.h"
#include "json.hpp"
#include "Util.h"

using json = nlohmann::json;

// String Splitting testJson

TEST(clientSplitTest, splitStringIncludeDelim) {
    std::string testString = "{a}{b}{c}";
    std::string delimiter = "}";

    std::vector<std::string> expectedList;
    expectedList.push_back("{a}");
    expectedList.push_back("{b}");
    expectedList.push_back("{c}");

    EXPECT_EQ( expectedList, splitString(testString, delimiter, true) );
}

TEST(clientSplitTest, splitStringExcludeDelim) {
    std::string testString = "a-b-c";
    std::string delimiter = "-";

    std::vector<std::string> expectedList;
    expectedList.push_back("a");
    expectedList.push_back("b");
    expectedList.push_back("c");

    EXPECT_EQ( expectedList, splitString(testString, delimiter, false) );
}

TEST(clientSplitTest, splitStringExcludeNewline) {
    std::string testString = "a\nb\nc";
    std::string delimiter = "\n";

    std::vector<std::string> expectedList;
    expectedList.push_back("a");
    expectedList.push_back("b");
    expectedList.push_back("c");

    EXPECT_EQ( expectedList, splitString(testString, delimiter, false) );
}

TEST(clientSplitTest, splitStringIncludeOnlyDelimiter) {
    std::string testString = "---";
    std::string delimiter = "-";

    std::vector<std::string> expectedList;
    expectedList.push_back("-");
    expectedList.push_back("-");
    expectedList.push_back("-");

    EXPECT_EQ( expectedList, splitString(testString, delimiter, true) );
}

TEST(clientSplitTest, splitStringExcludeOnlyDelimiter) {
    std::string testString = "---";
    std::string delimiter = "-";

    std::vector<std::string> expectedList;
    expectedList.push_back("");
    expectedList.push_back("");
    expectedList.push_back("");

    EXPECT_EQ( expectedList, splitString(testString, delimiter, false) );
}

// Message Creation Tests

json inputToJson(const std::string& testInput) {
    std::string serverMessage = makeServerMessage(testInput);
    json testJson = json::parse(serverMessage);
    return testJson;
}

TEST(clientOutgoingTest, createSessionMessage) {
    json testJson = inputToJson("!createsession sessionname");
    EXPECT_EQ( PMConstants::TYPE_CREATE_SESSION, testJson.at("command") );
    EXPECT_EQ( "sessionname", testJson.at("data") );
}

TEST(clientOutgoingTest, joinSessionMessage) {
    json testJson = inputToJson("!joinsession sessionname");
    EXPECT_EQ( PMConstants::TYPE_JOIN_SESSION, testJson.at("command") );
    EXPECT_EQ( "sessionname", testJson.at("data") );
}

TEST(clientOutgoingTest, leaveSessionMessage) {
    json testJson = inputToJson("!leavesession");
    EXPECT_EQ( PMConstants::TYPE_LEAVE_SERVER, testJson.at("command") );
}

TEST(clientOutgoingTest, gameInputMessage) {
    json testJson = inputToJson("!gameinput gamename");
    EXPECT_EQ( PMConstants::TYPE_GAME_INPUT, testJson.at("command") );
    EXPECT_EQ( "gamename", testJson.at("data") );
}

TEST(clientOutgoingTest, whisperMessage) {
    json testJson = inputToJson("!whisper username");
    EXPECT_EQ( PMConstants::TYPE_WHISPER, testJson.at("command") );
    EXPECT_EQ( "username", testJson.at("data") );
}

TEST(clientOutgoingTest, requestGamesMessage) {
    json testJson = inputToJson("!requestgames");
    EXPECT_EQ( PMConstants::TYPE_REQUEST_GAMES, testJson.at("command") );
}

TEST(clientOutgoingTest, chatMessageExplicit) {
    json testJson = inputToJson("!chat this is a chat message");
    EXPECT_EQ( PMConstants::TYPE_CHAT, testJson.at("command") );
    EXPECT_EQ( "this is a chat message", testJson.at("data") );
}

TEST(clientOutgoingTest, chatMessageImplicit) {
    json testJson = inputToJson("this is a chat message");
    EXPECT_EQ( PMConstants::TYPE_CHAT, testJson.at("command") );
    EXPECT_EQ( "this is a chat message", testJson.at("data") );
}

TEST(clientOutgoingTest, invalidCommandMessage) {
    json testJson = inputToJson("!wrong this is a chat message");
    EXPECT_EQ( PMConstants::TYPE_CHAT, testJson.at("command") );
    EXPECT_EQ( "!wrong this is a chat message", testJson.at("data") );
}

TEST(clientOutgoingTest, invalidCommandMessageCapslock) {
    json testJson = inputToJson("!CREATESESSION this is a chat message");
    EXPECT_EQ( PMConstants::TYPE_CHAT, testJson.at("command") );
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
    EXPECT_EQ( ProcessConstants::RESPONSE_CREATESUCCESS, processedTestResponse );
}

TEST(clientIncomingTest, sessionCreateFail) {
    std::string testResponse = "{ \"command\" \" !createsession\", \"data\": \"fail\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( ProcessConstants::RESPONSE_CREATEFAIL, processedTestResponse );
}

TEST(clientIncomingTest, sessionJoinSuccess) {
    std::string testResponse = "{ \"command\" \" !joinsession\", \"data\": \"sessionname\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    std::stringstream expectedResponse;
    expectedResponse << ProcessConstants::RESPONSE_JOINSUCCESS << "sessionname";
    EXPECT_EQ( expectedResponse, processedTestResponse );
}

TEST(clientIncomingTest, sessionJoinFail) {
    std::string testResponse = "{ \"command\" \" !joinsession\", \"data\": \"fail\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( ProcessConstants::RESPONSE_JOINFAIL, processedTestResponse );
}

TEST(clientIncomingTest, sessionLeave) {
    std::string testResponse = "{ \"command\" \" !leavesession\", \"data\": \"sessionname\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    std::stringstream expectedResponse;
    expectedResponse << ProcessConstants::RESPONSE_LEAVE << "sessionname";
    EXPECT_EQ( expectedResponse, processedTestResponse );
}

TEST(clientIncomingTest, gameResponse) {
    std::string testResponse = "{ \"command\" \" !gameinput\", \"data\": \"this is game data\" }";
    std::string processedTestResponse = processServerMessage(testResponse);
    EXPECT_EQ( "this is game data", processedTestResponse );
}
