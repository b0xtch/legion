#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <vector>

class SessionManager {
public:
  SessionManager();
  void createNewSession();
  void addConnection(networking::Connection connection);
  void sendMessage(networking::Connection connection);
  Session getSessionForConnection(networking::Connection connection);
private:
  std::vector<Session> sessions;
  std::vector<networking::Connection> unassignedConnections;
};


#endif
