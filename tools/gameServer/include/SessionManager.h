#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <set>
#include <map>
#include "Server.h" 
#include "Session.h"
#include "User.h"

using networking::Server;
using networking::Connection;
using networking::Message;

/**
 * This class is responsible for managing multiple sessionss
 * **/
class SessionManager {

public:
  SessionManager(int maxSessions);
  
  int getMaxSessions();

  Session createNewSession();
  
  void addToSession(const Connection& connectionToAdd, std::string& sessionId);

  std::vector<Message> processMessage(const Message& message);
  
  std::vector<Message> constructMessage(const std::string& message, std::set<User>& users);

  std::vector<Message> constructMessage(const std::string& message, std::set<Connection>& user);
  
  void addConnection(const Connection& connection);

  void removeConnection(const Connection& connection);
  
  void sendMessage(const Connection& connection);
  
  std::set<Connection> getUnassignedConnections();
  
  Session getSessionForConnection(const Connection& connection);

private:
  std::map<std::string, Session> sessions;
  
  std::set<Connection> unassignedConnections;
  
  int MAX_SESSIONS_PER_SERVER;
};


#endif
