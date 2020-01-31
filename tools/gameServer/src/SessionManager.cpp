
class SessionManager {
public:
  SessionManager(std::string sessionId, Connection c);
  Session createNewSession(std::string sessionId);
private:
  std::vector<Session> sessions;
};


namespace SessionManager{
    Session createNewSession(){
        Session::Session session();
        sessions.push_back(session);
        return session;
    };
    
}

