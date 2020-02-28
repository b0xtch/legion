#include "ParsedMessage.h"

#include "json.hpp"



ParsedMessage::Type ParsedMessage::getType() const {
    return type;
}

std::string ParsedMessage::getData() const {
    return data;
}

ParsedMessage ParsedMessage::interpret(const std::string& text) {
    using json = nlohmann::json;
    
    json jsonObj;
    std::string msgType;
    std::string msgData;
    try {
        jsonObj = json::parse(text);
        msgType = jsonObj[PMConstants::KEY_COMMAND].get<std::string>();
        msgData = jsonObj[PMConstants::KEY_DATA].get<std::string>();
    }
    catch (json::parse_error& e) {
        return {Type::Invalid, "JSON Parse error"};
    }
    catch (json::type_error& e) {
        return {Type::Invalid, "JSON Type error"};
    }
    
    ParsedMessage ret;
    if (msgType.compare(PMConstants::TYPE_SERVER_STOP) == 0) {
        ret = {Type::ServerStop, msgData};
    }
    else if (msgType.compare(PMConstants::TYPE_CREATE_SESSION) == 0) {
        ret = {Type::CreateSession, msgData};
    }
    else if (msgType.compare(PMConstants::TYPE_JOIN_SESSION) == 0) {
        ret = {Type::JoinSession, msgData};
    }
    else if (msgType.compare(PMConstants::TYPE_LEAVE_SERVER) == 0) {
        ret = {Type::LeaveServer, msgData};
    }
    else if (msgType.compare(PMConstants::TYPE_CHAT) == 0) {
        ret = {Type::Chat, msgData};
    }
    else if (msgType.compare(PMConstants::TYPE_LIST_GAMES) == 0) {
        ret = {Type::ListGames, msgData};
    }
    else {
        ret = {Type::Other, msgData};
    }
    
    return ret;
}

ParsedMessage::ParsedMessage() : type{ParsedMessage::Type::Invalid}, data{""}
{}

ParsedMessage::ParsedMessage(ParsedMessage::Type type, const std::string& data) : type{type}, data{data}
{}