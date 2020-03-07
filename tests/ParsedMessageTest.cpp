#include <utility>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ParsedMessage.h"
#include "json.hpp"

std::string makeMessageString(const std::string& command, const std::string& data) {
    return "{\"command\":\"" + command + "\", \"data\":\"" + data + "\"}";
}

std::pair<std::string, std::string> msgTextToPair(const std::string& msgText) {
    using json = nlohmann::json;
    
    json jsonObj = json::parse(msgText);
    
    return {jsonObj["command"].get<std::string>(), jsonObj["data"].get<std::string>()};
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
    
    pm = ParsedMessage::interpret(makeMessageString(PMConstants::TYPE_WHISPER, "{\\\"msg\\\":\\\"we are updating our privacy policy\\\"}"));
    EXPECT_EQ(ParsedMessage::Type::Whisper, pm.getType());
    EXPECT_EQ("{\"msg\":\"we are updating our privacy policy\"}", pm.getData());
    
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

TEST(ParsedMessageTests, makeMsg_enum) {
    std::string text = ParsedMessage::makeMsgText(ParsedMessage::Type::ServerStop, "stop! hammer time!");
    auto [command, data] = msgTextToPair(text);
    EXPECT_EQ(PMConstants::TYPE_SERVER_STOP, command);
    EXPECT_EQ("stop! hammer time!", data);
    
    // WIP
}

TEST(ParsedMessageTests, makeMsg_enumBad) {
    EXPECT_THROW(ParsedMessage::makeMsgText(ParsedMessage::Type::Invalid, "some data"), std::runtime_error);
    EXPECT_THROW(ParsedMessage::makeMsgText(ParsedMessage::Type::Other, "some data"), std::runtime_error);
    EXPECT_THROW(ParsedMessage::makeMsgText(static_cast<ParsedMessage::Type>(-1), "some data"), std::runtime_error);
}

TEST(ParsedMessageTests, makeMsg_string) {
    std::string text = ParsedMessage::makeMsgText("this is a custom command!", "stop! hammer time!");
    auto [command, data] = msgTextToPair(text);
    EXPECT_EQ("this is a custom command!", command);
    EXPECT_EQ("stop! hammer time!", data);
}