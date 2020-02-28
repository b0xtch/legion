#ifndef PARSED_MESSAGE_H
#define PARSED_MESSAGE_H

#include <string>

class ParsedMessage {
public:
    enum Type {
        Invalid,
        Other,
        ServerStop,
        CreateSession,
        JoinSession,
        LeaveServer
    };
    
    /** Determines the type of message received from a client. */
    ParsedMessage interpretType(const std::string& text);
    
    Type type;
    std::string data;
    
private:
    ParsedMessage();
    ParsedMessage(Type type, const std::string& data);
};

#endif