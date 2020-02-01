#include "SessionManager.h"


#include <algorithm>

namespace SessionManager{
    void createNewSession(){
        Session::Session session();
        sessions.push_back(session);
        return session;
    };

    void addConnection(networking::Connection connection){
      unassignedConnections.push_back(connection);
    };

    void addToSession(networking::Connection connectionToAdd, std::string sessionId){
      auto connectionIter = find_if(
        unassignedConnections.begin(), unassignedConnections.end(), [=](const networking::Connection &connection){
          return connection == connectionToAdd;
          }
      );

      if (connectionIter == unassignedConnections.end()){
          throw std::invalid_argument("No connection exists");
      };

      auto sessionIter = find_if(
        sessions.begin(), sessions.end(), [=](const Session &session){
          return session.getSessionId() == sessionId;
        }
      );

      if (sessionIter == sessions.end()){
          throw std::invalid_argument("No connection exists");
      };

      sessionIter->addClient(connectionToAdd);
    };

    void sendMessage(networking::Connection connection, std::string message){
      Session session = getSessionForConnection(connection);
      session.broadCastMessage(message);

    }

    Session getSessionForConnection(networking::Connection connection){
      auto it = find_if(
        sessions.begin(), sessions.end(), [=](const Session &session){
          return session.isClient(connection);
        }
      );

      if(it != sessions.end()){
        return *it;
      }

    }

    
}

