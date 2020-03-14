#include "SessionManager.h"
#include "ParsedMessage.h"
#include <algorithm>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

/**
 * Session manager constructor that takes in max number of session allowed
 * **/
SessionManager::SessionManager(int maxSessions): MAX_SESSIONS_PER_SERVER{maxSessions} {}


/**
 * function for creating a new session
 * **/
Session SessionManager::createNewSession(){
    if(sessions.size() <= MAX_SESSIONS_PER_SERVER){
        Session session = Session{};
        sessions[session.getSessionId()] = session;
        return session;
    };
    throw; // ServerLimitReached();
};


/**
 * Get max session 
 * **/
int SessionManager::getMaxSessions(){
    return MAX_SESSIONS_PER_SERVER;
}


/**
 * Function for adding a new connection which is not part of any session
 * **/
void SessionManager::addConnection(const Connection& connection){
    unassignedConnections.insert(connection);
};


/**
 * Function for removing a connection
 * **/
void SessionManager::removeConnection(const Connection& connection){
    if(unassignedConnections.find(connection) != unassignedConnections.end()){
        unassignedConnections.erase(connection);
    }
}

/**
 * Function for adding connection to existing session
 * **/
void SessionManager::addToSession(const Connection& connectionToAdd, std::string& sessionId){
    sessions[sessionId].addUser(connectionToAdd);
};


/**
 * Find session for particular connection
 * **/  
Session SessionManager::getSessionForConnection(const Connection& connection){
    auto it = find_if(sessions.begin(), sessions.end(), [&](const std::pair<std::string, Session> &map){
        return map.second.isUser(connection);
    });
    if(it != sessions.end()){
        return it->second;
    }
}


/**
 * This function constructs vector of struct message for all passed connections
 * **/
std::vector<Message> SessionManager::constructMessage(const std::string& message, std::set<User>& users){
  std::vector<Message> messages;
  for(auto user: users){
    messages.push_back(
      Message{user.getConnection(), message}
    );
  }
  return messages;
}


/**
 * This function recieves messages and returns new vector of
 * Message struct which will be used by server to forward messages 
 * **/
std::vector<Message> SessionManager::processMessage(const Message& message){
    ParsedMessage parsedMessage = ParsedMessage::interpret(message.text);

    ParsedMessage::Type type = parsedMessage.getType();
    std::string msg = parsedMessage.getData();

    if(type == ParsedMessage::Type::CreateSession){
        Session session = createNewSession();
        session.addUser(message.connection);
        std::set<User> users = session.getAllUsers();
        return constructMessage("New Player Joined", users);
   
    } else if(type == ParsedMessage::Type::Chat){
        Session session = getSessionForConnection(message.connection);
        std::set<User> users = session.getAllUsers();
        return constructMessage(message.text, users);

    } else if(type == ParsedMessage::Type::LeaveSession){
        Session session = getSessionForConnection(message.connection);
        session.removeUser(message.connection);
        std::set<User> users = session.getAllUsers();
        return constructMessage("Player Left", users);
    } 
}  


