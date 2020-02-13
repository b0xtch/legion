#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <vector>
#include <unordered_map>
#include "Server.h" 
#include "Session.h"

using networking::Server;
using networking::Connection;
using networking::Message;

/**
 * This class is responsible for managing multiple sessionss
 * **/
class SessionManager {

public:
  SessionManager(int maxSessions);

  Session createNewSession();
  
  void addToSession(const Connection& connectionToAdd, std::string& sessionId);

  std::vector<Message> processMessage(const Message& message);
  
  std::vector<Message> constructMessage(const std::string& message, std::unordered_map<ConnectionId, Connection>& connections);
  
  void addConnection(const Connection& connection);

  void removeConnection(const Connection& connection);
  
  void sendMessage(const Connection& connection);
  
  Session getSessionForConnection(const Connection& connection);

private:
  std::unordered_map<std::string, Session> sessions;
  
  std::vector<Connection> unassignedConnections;
  
  int MAX_SESSIONS_PER_SERVER;
};


#endif
