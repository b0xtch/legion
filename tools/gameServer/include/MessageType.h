#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <string>

namespace MessageType {
    enum Type {
        Invalid,
        Other,
        ServerStop,
        CreateSession,
        JoinSession,
        LeaveServer
    };
    
    /** Determines the type of message received from a client. */
    Type interpretType(const std::string& text);
}

#endif