#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include <unordered_set>

// class Game;
class Session {
  // friend class Game;
public:
<<<<<<< HEAD
    Session(int ID, Game& game, Player& sessionCreator);
    ~Session();

    void updateTime();
    void checkTimeOut();

    void startGame();	
	void endGame();
    void advanceGameTurn(); //state machine???
    void declareWinner();

    void addPlayer(Player& newPlayer);
    void removePlayer(int removePlayerID);

private:
    Game sessionGame; //each session should only handle 1 Game at a time for now
    std::vector<Player> sessionPlayers;

    int sessionID;
    double maxIdleTime = 30;
    std::chrono::system_clock::time_point lastUpdateTime;

};
=======
    Session();
    std::vector<networking::Connection> getAllClients();
    networking::Connection getClient(uintptr_t id);
    void addClient(networking::Connection connection);
    bool isClient(networking::Connection connection);
    void broadCastMessage(std::string message);
    std::string getSessionId();
private:
    std::unordered_set<networking::Connection> clients;
    std::string sessionId;
    int sessionClientLimit;
}
>>>>>>> 043e60730ab5c04021be8f0f471e821e25ee1540

#endif
