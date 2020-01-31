#ifndef SESSION_H
#define SESSION_H

using networking::Server
using networking::Connection

class SessionManager {
public:
  SessionManager();
  Session createNewSession();
private:
  std::vector<Session> sessions;
};


#endif
