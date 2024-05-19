#ifndef BOMB_H
#define BOMB_H

#include "Weapon.h"
#include <iostream>

class Bomb : public Weapon {
private:
    int x, y;
    int explosionRange;
    static int nrOfBombs;

public:
    Bomb(int x, int y, int explosionRange);
    ~Bomb();

    static void decNrOfBombs();
    static void atention();

    int getX() const;
    int getY() const;
    int getExplosionRange() const;
    void setExplosionRange(int newRange);

    bool isInRange(int targetX, int targetY) override;
    friend std::ostream& operator<<(std::ostream& os, const Bomb& bomb);
};

#endif // BOMB_H
