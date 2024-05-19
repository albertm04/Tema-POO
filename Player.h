#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Map.h"
#include "Bomb.h"

class Player {
protected:
    std::string name;
    bool isAlive;
    int x, y;
    Map& map;

public:
    Player(const std::string& name, int startX, int startY, Map& map);
    virtual ~Player();
    virtual void move(int deltaX, int deltaY);
    void placeBomb(std::vector<Bomb>& bombs, int explosionRange);
    void die();

    int getX() const;
    int getY() const;
    std::string getName() const;

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

#endif // PLAYER_H
