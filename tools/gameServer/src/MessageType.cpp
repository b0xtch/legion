#include "MessageType.h"

#include "json.hpp"

namespace MessageType {
    Type interpretType(const std::string& text) {
        using json = nlohmann::json;
    
        json jsonObj = json::parse(text);
        
        // Message format is currently unknown so some placeholder strings are in place.
        std::string msgType = jsonObj["type"];
        
        Type ret;
        if (msgType.compare("serverStop")) {
            ret = Type::ServerStop;
        }
        else if (msgType.compare("create")) {
            ret = Type::CreateSession;
        }
        else if (msgType.compare("join")) {
            ret = Type::JoinSession;
        }
        else {
            ret = Type::Other;
        }
        
        return ret;
    }
}