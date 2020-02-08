#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <string>

namespace MessageType {
    enum Type {
        Other,
        ServerStop,
        CreateSession,
        JoinSession
    };
    
    /** Determines the type of message received from a client. */
    Type interpretType(const std::string& text);
}

#endif