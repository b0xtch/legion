#ifndef PARSED_MESSAGE_H
#define PARSED_MESSAGE_H

#include <string>
#include <vector>
#include <utility>

#include "json.hpp"

namespace PMConstants {
    const std::string KEY_COMMAND = "command";
    const std::string KEY_DATA = "data";
    
    const std::string TYPE_SERVER_STOP = "serverstop";
    const std::string TYPE_CREATE_SESSION = "!createsession";
    const std::string TYPE_JOIN_SESSION = "!joinsession";
    const std::string TYPE_LEAVE_SERVER = "!leaveserver";
    const std::string TYPE_LEAVE_SESSION = "!leavesession";
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
    
    /** Builds a string that should be used as the message.text of a networking::Message.
     * Throws a runtime exception if msgCommand does not match any Type enumeration OR if it matches "Invalid" or "Other". */
    static std::string makeMsgText(Type msgCommand, const std::string& msgData);
    
    /** Builds a string that should be used as the message.text of a networking::Message. */
    static std::string makeMsgText(const std::string& msgCommand, const std::string& msgData);
    
    
private:
    ParsedMessage();
    ParsedMessage(Type type, const std::string& data);
    
    Type type;
    std::string data;
    
    /** Used to make sure we don't initialize the pairs more than once. */
    static bool initialized;
    
    /** Stores pairs of the command string and the enum they represent. */
    static std::vector<std::pair<std::string, Type>> commandAndTypePairs;
    
    static void initializePairs();
};

#endif
