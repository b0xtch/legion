#ifndef PARSED_MESSAGE_H
#define PARSED_MESSAGE_H

#include <string>

namespace PMConstants {
    const std::string KEY_COMMAND = "command";
    const std::string KEY_DATA = "data";
    
    const std::string TYPE_SERVER_STOP = "serverstop";
    const std::string TYPE_CREATE_SESSION = "create";
    const std::string TYPE_JOIN_SESSION = "join";
    const std::string TYPE_LEAVE_SERVER = "leave";
    const std::string TYPE_CHAT = "chat";
    const std::string TYPE_LIST_GAMES = "listgames";
};

class ParsedMessage {
public:
    enum Type {
        Invalid,
        Other,
        ServerStop,
        CreateSession,
        JoinSession,
        LeaveServer,
        Chat,
        ListGames
    };
    
    Type getType() const;
    std::string getData() const;
    
    /** Determines the type of message received from a client. */
    static ParsedMessage interpret(const std::string& text);
    
private:
    ParsedMessage();
    ParsedMessage(Type type, const std::string& data);
    
    Type type;
    std::string data;
};

#endif