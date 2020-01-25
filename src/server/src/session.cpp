session(int ID, Game& game, Player& sessionCreator) {
    sessionID = ID;
    sessionGame = game;
    sessionPlayers.push_back(sessionCreator);
}

void updateTime() {
    lastUpdateTime = system_clock()::now();
}

void checkTimeOut() {
    auto currentTime = system_clock()::now();
    std::chrono::duration<double> elapsedTime = currentTime - lastUpdateTime;

    if ( (elapsedTime.count()/60) > maxIdleTime ) endGame();
}

void startGame() {
    //Unfinished
}

void endGame() {
    //Unfinished
    declareWinner();
}

void advanceGameTurn() {
    //Unfinished
    updateTime();
}

void declareWinner() {
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

void addPlayer(Player& newPlayer) {
    sessionPlayers.push_back(newPlayer);
}

void removePlayer(int removePlayerID) {
    for (int i = 0; i < sessionPlayers.size(); i++) {
        if ( sessionPlayers.at(i).getID() == removePlayerID ) {
            sessionPlayers.erase( sessionPlayers.begin()+i );
            break;
        }
    }

    if ( sessionPlayers.size() == 0 ) endGame();
}
