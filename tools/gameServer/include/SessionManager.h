#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <vector>
#include <networking>

/**
 * This class is responsible for managing multiple sessionss
 * **/
class SessionManager {

public:
  SessionManager(int maxSessions);

  void createNewSession();
  
  vector<networking::Message> update(const networking::Message& message);
  
  vector<networking::Message> constructMessage(const std::string& message, std::vector<networking::Connection>& connections);
  
  void addConnection(const networking::Connection& connection);
  
  void sendMessage(const networking::Connection& connection);
  
  Session getSessionForConnection(const networking::Connection& connection);

private:
  std::vector<Session> sessions;
  
  std::vector<networking::Connection> unassignedConnections;
  
  int MAX_SESSIONS_PER_SERVER;
};


#endif
