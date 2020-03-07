#ifndef PARSED_MESSAGE_H
#define PARSED_MESSAGE_H

#include <string>

namespace PMConstants {
    const std::string KEY_COMMAND = "command";
    const std::string KEY_DATA = "data";
    
    const std::string TYPE_SERVER_STOP = "serverstop";
    const std::string TYPE_CREATE_SESSION = "!createsession";
    const std::string TYPE_JOIN_SESSION = "!joinsession";
    const std::string TYPE_LEAVE_SERVER = "!leavesession";
    const std::string TYPE_CHAT = "!chat";
    const std::string TYPE_WHISPER = "!whisper";
    const std::string TYPE_LIST_GAMES = "!requestgames";
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
        LeaveSession,
        Chat,
        Whisper,
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
