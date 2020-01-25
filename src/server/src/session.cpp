void updateTime() {
    lastUpdateTime = system_clock()::now();
}

void checkTimeOut() {
    auto currentTime = system_clock()::now();
    std::chrono::duration<double> elapsedTime = currentTime - lastUpdateTime;

    if ( (elapsedTime.count()/60) > maxIdleTime ) endGame();
}

void startGame() {}
void endGame() {}

void advanceGameTurn() {
    updateTime();
}

void declareWinner() {
    int winnerID;
    int maxPoints;

    for (auto player : sessionPlayers) {
        if (player.getPoints() > maxPoints) {
            maxPoints = player.getPoints();
            winnerID = player.getID();
        }
    }
    
    std::cout << "Player " << winnerID << " is the winner!";
}

int main(){}
