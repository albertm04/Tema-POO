#include "Game.h"
#include <fstream>

Game::Game() : gameMap(std::make_unique<Map>()) {}

Game::~Game() {
    // Cleanup if necessary
}

void Game::setMap(std::istream& is) {
    is >> *gameMap;
}

Map& Game::getMap() {
    return *gameMap;
}

void Game::addPlayer(const Player& player) {
    players.push_back(player);
    gameMap->updateCell(player.getX(), player.getY(), player.getName()[0]);
}

std::vector<Player>& Game::getPlayers() {
    return players;
}

std::vector<Bomb>& Game::getBombs() {
    return bombs;
}

void Game::explodeAllBombs() {
    for (auto& bomb : bombs) {
        Bomb::decNrOfBombs();
        gameMap->updateCell(bomb.getX(), bomb.getY(), 'X');
        for (auto& player : players) {
            if (bomb.isInRange(player.getX(), player.getY())) {
                player.die();
            }
        }
    }
    bombs.clear();
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << *(game.gameMap);
    for (const auto& player : game.players) {
        os << player << "\n";
    }
    for (const auto& bomb : game.bombs) {
        os << bomb << "\n";
    }
    return os;
}

