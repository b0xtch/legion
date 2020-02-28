#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ParsedMessage.h"

std::string makeMessageString(const std::string& command, const std::string& data) {
    return "{\"command\":\"" + command + "\", \"data\":\"" + data + "\"}";
}

TEST(ParsedMessageTests, interpret) {
    ParsedMessage pm = ParsedMessage::interpret(makeMessageString(PMConstants::TYPE_SERVER_STOP, "Stop! Hammer time!"));
    EXPECT_EQ(ParsedMessage::Type::ServerStop, pm.getType());
    EXPECT_EQ("Stop! Hammer time!", pm.getData());
    
    pm = ParsedMessage::interpret(makeMessageString(PMConstants::TYPE_CREATE_SESSION, "{\\\"gameId\\\":\\\"kahoot2\\\"}"));
    EXPECT_EQ(ParsedMessage::Type::CreateSession, pm.getType());
    EXPECT_EQ("{\"gameId\":\"kahoot2\"}", pm.getData());
    
    pm = ParsedMessage::interpret(makeMessageString(PMConstants::TYPE_JOIN_SESSION, "{\\\"sessionId\\\":\\\"abc123\\\"}"));
    EXPECT_EQ(ParsedMessage::Type::JoinSession, pm.getType());
    EXPECT_EQ("{\"sessionId\":\"abc123\"}", pm.getData());
    
    pm = ParsedMessage::interpret(makeMessageString(PMConstants::TYPE_LEAVE_SERVER, ""));
    EXPECT_EQ(ParsedMessage::Type::LeaveServer, pm.getType());
    EXPECT_EQ("", pm.getData());
    
    pm = ParsedMessage::interpret(makeMessageString(PMConstants::TYPE_CHAT, "{\\\"msg\\\":\\\"can u start already?? i have to study soon\\\"}"));
    EXPECT_EQ(ParsedMessage::Type::Chat, pm.getType());
    EXPECT_EQ("{\"msg\":\"can u start already?? i have to study soon\"}", pm.getData());
    
    pm = ParsedMessage::interpret(makeMessageString(PMConstants::TYPE_LIST_GAMES, ""));
    EXPECT_EQ(ParsedMessage::Type::ListGames, pm.getType());
    EXPECT_EQ("", pm.getData());
    
    pm = ParsedMessage::interpret(makeMessageString("some other command", "{\\\"throw\\\":\\\"paper\\\"}"));
    EXPECT_EQ(ParsedMessage::Type::Other, pm.getType());
    EXPECT_EQ("{\"throw\":\"paper\"}", pm.getData());
}

TEST(ParsedMessageTests, interpretBadJson) {
    ParsedMessage pm = ParsedMessage::interpret("{\"command\"::\"\",\"data\":\"\"}");
    ASSERT_EQ(ParsedMessage::Type::Invalid, pm.getType());
}

TEST(ParsedMessageTests, interpretBadKeys) {
    ParsedMessage pm = ParsedMessage::interpret("{\"cmd\":\"some other command\",\"data\":\"\"}");
    ASSERT_EQ(ParsedMessage::Type::Invalid, pm.getType());
}