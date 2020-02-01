Game GameManager::createNewGame() {
    if (Games.size() < maxGames) {
        Game newgame();
        Games.push_back(newgame);
    }
}

void GameManager::startGame(std::string name) {
}

void GameManager::endGame(std::string name) {
}

void GameManager::advanceGame(std::string name) {
    auto selectedGame = std::find_if(Games.begin(), Games.end(), [name] (auto& game) { return game.getName() == name; });
    selectedGame.advanceTurn();
}
