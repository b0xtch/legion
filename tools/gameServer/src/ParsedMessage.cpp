#include "ParsedMessage.h"

#include <algorithm>

#include "json.hpp"

ParsedMessage::Type ParsedMessage::getType() const {
    return type;
}

std::string ParsedMessage::getData() const {
    return data;
}

ParsedMessage ParsedMessage::interpret(const std::string& text) {
    initializePairs();
    
    using json = nlohmann::json;
    
    json jsonObj;
    std::string msgTypeString;
    std::string msgDataString;
    try {
        jsonObj = json::parse(text);
        msgTypeString = jsonObj[PMConstants::KEY_COMMAND].get<std::string>();
        msgDataString = jsonObj[PMConstants::KEY_DATA].get<std::string>();
    }
    catch (json::parse_error& e) {
        return {Type::Invalid, "JSON Parse error"};
    }
    catch (json::type_error& e) {
        return {Type::Invalid, "JSON Type error"};
    }
    
    auto itr = std::find_if(commandAndTypePairs.begin(), commandAndTypePairs.end(),
        [&msgTypeString] (const std::pair<std::string, Type>& pair) {
            return msgTypeString.compare(pair.first) == 0;
        }
    );
    Type msgType = Type::Other;
    if (itr != commandAndTypePairs.end()) {
        msgType = itr->second;
    }
    
    return {msgType, msgDataString};
}

// ^^^ PUBLIC
// vvv PRIVATE

ParsedMessage::ParsedMessage() : type{ParsedMessage::Type::Invalid}, data{""}
{}

ParsedMessage::ParsedMessage(ParsedMessage::Type type, const std::string& data) : type{type}, data{data}
{}

bool ParsedMessage::initialized = false;

std::vector<std::pair<std::string, ParsedMessage::Type>> ParsedMessage::commandAndTypePairs{};

void ParsedMessage::initializePairs() {
    if (!initialized) {
        commandAndTypePairs.clear();
        commandAndTypePairs.push_back({PMConstants::TYPE_SERVER_STOP, ParsedMessage::Type::ServerStop});
        commandAndTypePairs.push_back({PMConstants::TYPE_CREATE_SESSION, ParsedMessage::Type::CreateSession});
        commandAndTypePairs.push_back({PMConstants::TYPE_JOIN_SESSION, ParsedMessage::Type::JoinSession});
        commandAndTypePairs.push_back({PMConstants::TYPE_LEAVE_SERVER, ParsedMessage::Type::LeaveServer});
        commandAndTypePairs.push_back({PMConstants::TYPE_CHAT, ParsedMessage::Type::Chat});
        commandAndTypePairs.push_back({PMConstants::TYPE_WHISPER, ParsedMessage::Type::Whisper});
        commandAndTypePairs.push_back({PMConstants::TYPE_LIST_GAMES, ParsedMessage::Type::ListGames});
        
        initialized = true;
    }
}