#include "ParsedMessage.h"

#include "json.hpp"

ParsedMessage ParsedMessage::interpretType(const std::string& text) {
    using json = nlohmann::json;
    
    json jsonObj;
    std::string msgType;
    std::string msgData;
    try {
        jsonObj = json::parse(text);
        msgType = jsonObj["command"];
        msgData = jsonObj["data"];
    }
    catch (json::parse_error& e) {
        return {Type::Invalid, ""};
    }
    catch (json::type_error& e) {
        return {Type::Invalid, ""};
    }
    
    // Message format is currently unknown so some placeholder strings are in place.
    
    ParsedMessage ret;
    if (msgType.compare("serverStop") == 0) {
        ret = {Type::ServerStop, data};
    }
    else if (msgType.compare("create") == 0) {
        ret = {Type::CreateSession, data};
    }
    else if (msgType.compare("join") == 0) {
        ret = {Type::JoinSession, data};
    }
    else if (msgType.compare("leave") == 0) {
        ret = {Type::LeaveServer, data};
    }
    else {
        ret = {Type::Other, data};
    }
    
    return ret;
}

ParsedMessage::ParsedMessage() : type{ParsedMessage::Type::Invalid}, data{""}
{}

ParsedMessage::ParsedMessage(ParsedMessage::Type type, const std::string& data) : type{type}, data{data}
{}