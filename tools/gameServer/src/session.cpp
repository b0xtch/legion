#include "session.h"
#include <utility>
using std::unique_ptr
Session::session(int ID, Game& game, Player& sessionCreator) {
    sessionID = ID;
    sessionGame = game; //current game
    sessionPlayers.push_back(sessionCreator);
}

void Session:updateTime() {
    lastUpdateTime = system_clock()::now();
}

void Session::checkTimeOut() {
    auto currentTime = system_clock()::now();
    std::chrono::duration<double> elapsedTime = currentTime - lastUpdateTime;

    if ( (elapsedTime.count()/60) > maxIdleTime ) endGame();
}

void Session::startGame() {
    //Unfinished
    //startGame already has a sessionGame which is a newGame parameter

}

void Session::endGame() {
    //Unfinished
    declareWinner();
}

void Session::advanceGameTurn() {
    //Unfinished
    updateTime();
    for_each(sessionPlayers.begin(), sessionPlayers.end(), [&] Player player {
      player.getPlayerTurn() = make_unique<gameTurn>(new gameTurn); //each player has a unique turn for them when session is created
    });
    //advanceGameTurn will transition between game states
    //and update on players' points and display all info to players
    sessionGame.displayInfo(); //end of turn

}

void Session::declareWinner() {
    int winnerID;
    int maxPoints;

    for (auto Aplayer : sessionPlayers) {
        if (Aplayer.getPoints() > maxPoints) {
            maxPoints = Aplayer.getPoints();
            winnerID = Aplayer.getID();
        }
    }

    std::cout << "Player " << winnerID << " is the winner!";
}

void Session::addPlayer(Player& newPlayer) {
    sessionPlayers.push_back(newPlayer);
}

void Session::removePlayer(int removePlayerID) {
    for (int i = 0; i < sessionPlayers.size(); i++) {
        if ( sessionPlayers.at(i).getID() == removePlayerID ) {
            sessionPlayers.erase( sessionPlayers.begin()+i );
            break;
        }
    }

    if ( sessionPlayers.size() == 0 ) endGame();
}
