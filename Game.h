#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Bomb.h"

class Game {
private:
    std::unique_ptr<Map> gameMap;
    std::vector<Player> players;
    std::vector<Bomb> bombs;

public:
    Game();
    ~Game();

    void setMap(std::istream& is);
    Map& getMap();
    void addPlayer(const Player& player);
    std::vector<Player>& getPlayers();
    std::vector<Bomb>& getBombs();
    void explodeAllBombs();

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};

#endif // GAME_H

