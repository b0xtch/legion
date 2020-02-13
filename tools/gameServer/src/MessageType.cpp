#include "MessageType.h"

#include "json.hpp"

namespace MessageType {
    
    Type interpretType(const std::string& text) {
        using json = nlohmann::json;
        
        json jsonObj;
        std::string msgType;
        try {
            jsonObj = json::parse(text);
            msgType = jsonObj["type"];
        }
        catch (json::parse_error& e) {
            return Type::Invalid;
        }
        catch (json::type_error& e) {
            return Type::Invalid;
        }
        
        // Message format is currently unknown so some placeholder strings are in place.
        
        Type ret;
        if (msgType.compare("serverStop") == 0) {
            ret = Type::ServerStop;
        }
        else if (msgType.compare("create") == 0) {
            ret = Type::CreateSession;
        }
        else if (msgType.compare("join") == 0) {
            ret = Type::JoinSession;
        }
        else if (msgType.compare("leave") == 0) {
            ret = Type::LeaveServer;
        }
        else {
            ret = Type::Other;
        }
        
        return ret;
    }
}