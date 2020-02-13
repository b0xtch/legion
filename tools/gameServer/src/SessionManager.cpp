#include "SessionManager.h"
#include <algorithm>


/**
 * Session manager constructor that takes in max number of session allowed
 * **/
SessionManager::SessionManager(int maxSessions): MAX_SESSIONS_PER_SERVER{maxSessions} {}

/**
 * function for creating a new session
 * **/
void SessionManager::createNewSession(){
  if(sessions.size() <= MAX_SESSIONS_PER_SERVER){
    Session session = Session{};
    sessions.push_back(session);
  };

  throw; // ServerLimitReached();
};

/**
 * Function for adding a new connection which is not part of any session
 * **/
void SessionManager::addConnection(const Connection& connection){
  unassignedConnections.push_back(connection);
};

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

  auto sessionIter = find_if(
    sessions.begin(), sessions.end(), [&](Session &session){
      return session.getSessionId() == sessionId;
    }
  );

  if (sessionIter == sessions.end()){
      throw; // SessionNotFound();
  };

  sessionIter->addClient(connectionToAdd);
};


/**
 * Find session for particular connection
 * **/  
Session SessionManager::getSessionForConnection(const Connection& connection){
  auto it = find_if(
    sessions.begin(), sessions.end(), [&](Session &session){
      return session.isClient(connection);
    }
  );

  if(it != sessions.end()){
    return *it;
  }

}


/**
 * This function constructs vector of struct message for all passed connections
 * **/
std::vector<Message> SessionManager::constructMessage(const std::string& message, std::unordered_map<ConnectionId, Connection>& connections){
  std::vector<Message> messages;
  for(auto connection: connections){
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
  Session session = getSessionForConnection(message.connection);
  return constructMessage(message.text, session.getAllClients());
}  


