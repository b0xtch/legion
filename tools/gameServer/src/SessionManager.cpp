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
  unassignedConnections.push_back(connection);
};

/**
 * Function for removing a connection
 * **/
void SessionManager::removeConnection(const Connection& connection){
    auto connectionIter = find_if(
            unassignedConnections.begin(), unassignedConnections.end(), [&](const Connection &connection){
            return connection == connection;
        }
    );
    if(connectionIter == unassignedConnections.end()){
        unassignedConnections.erase(connectionIter);
    }
}

/**
 * Function for adding connection to existing session
 * **/
void SessionManager::addToSession(const Connection& connectionToAdd, std::string& sessionId){
    auto connectionIter = find_if(
            unassignedConnections.begin(), unassignedConnections.end(), [&](const Connection &connection){
            return connection == connectionToAdd;
        }
    );
    
    if (connectionIter == unassignedConnections.end()){
        throw; // ConnectionNotFound();
    };
    
    if (sessions.find(sessionId) == sessions.end()){
        throw; // SessionNotFound();
    };
    
    std::cout<<"Session found"<< std::endl;
    sessions[sessionId].addClient(connectionToAdd);
};


/**
 * Find session for particular connection
 * **/  
Session& SessionManager::getSessionForConnection(const Connection& connection){
  auto it = find_if(
    sessions.begin(), sessions.end(), [&](std::unordered_map<std::string, Session>::value_type& v){
      return v.second.isClient(connection);
    }
  );
  // std::cout << "Get Session: " << it->first << std::endl; 
  if(it != sessions.end()){
    return it->second;
  }

}


/**
 * This function constructs vector of struct message for all passed connections
 * **/
std::vector<Message> SessionManager::constructMessage(const std::string& message, std::unordered_map<ConnectionId, Connection>& connections){
  std::vector<Message> messages;
  for(auto connection: connections){
    std::cout << "Connection: " << connection.first << std::endl;
    messages.push_back(
      Message{connection.second, message}
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
        session.addClient(message.connection);
        std::unordered_map<ConnectionId, Connection> connections = session.getAllClients();
        return constructMessage("New Player Joined", connections);
   
    } else if(type == ParsedMessage::Type::Chat){
        Session session = getSessionForConnection(message.connection);
        std::unordered_map<ConnectionId, Connection> connections = session.getAllClients();
        return constructMessage(message.text, connections);

    } else if(type == ParsedMessage::Type::LeaveSession){
        Session session = getSessionForConnection(message.connection);
        session.removeConnection(message.connection);
        std::unordered_map<ConnectionId, Connection> connections = session.getAllClients();
        return constructMessage("Player Left", connections);
    } 
}  


