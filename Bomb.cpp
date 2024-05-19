#include "Bomb.h"
#include <cmath>

int Bomb::nrOfBombs = 0;

Bomb::Bomb(int x, int y, int explosionRange) : x(x), y(y), explosionRange(explosionRange) {
    if (explosionRange < 1) {
        throw std::invalid_argument("Explosion range must be at least 1.");
    }
    nrOfBombs++;
    atention();
}

Bomb::~Bomb() {
    // Destructor logic if needed
}

void Bomb::decNrOfBombs() {
    nrOfBombs--;
    std::cout << "A bomb exploded! Remaining bombs: " << nrOfBombs << std::endl;
}

void Bomb::atention() {
    std::cout << "Attention, a bomb has been placed! Total unexploded bombs: " << nrOfBombs << std::endl;
}

int Bomb::getX() const {
    return x;
}

int Bomb::getY() const {
    return y;
}

int Bomb::getExplosionRange() const {
    return explosionRange;
}

void Bomb::setExplosionRange(int newRange) {
    explosionRange = newRange;
}

bool Bomb::isInRange(int targetX, int targetY) {
    return abs(targetX - x) <= explosionRange && abs(targetY - y) <= explosionRange;
}

std::ostream& operator<<(std::ostream& os, const Bomb& bomb) {
    os << "Bomb at (" << bomb.x << ", " << bomb.y << ") with range " << bomb.explosionRange;
    return os;
}
