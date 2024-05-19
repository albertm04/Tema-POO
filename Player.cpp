#include "Player.h"
#include "InvalidMoveException.h"
#include "Map.h"

Player::Player(const std::string& name, int startX, int startY, Map& map) : name(name), x(startX), y(startY), isAlive(true), map(map) {}

Player::~Player() {
    // Destructor logic if needed
}

void Player::move(int deltaX, int deltaY) {
    int newX = x + deltaX;
    int newY = y + deltaY;
    if (newX < 0 || newX >= map.getLayout().size() || newY < 0 || newY >= map.getLayout()[0].size()) {
        throw InvalidMoveException("Move out of map bounds!");
    }
    map.updateCell(newX, newY, name[0]);
    if (map.getLayout()[y][x] != 'x') {
        map.updateCell(x, y, '.');
    }
    x = newX;
    y = newY;
}

void Player::placeBomb(std::vector<Bomb>& bombs, int explosionRange) {
    bombs.push_back(Bomb(x, y, explosionRange));
    map.updateCell(x, y, 'x');
}

void Player::die() {
    if (isAlive) {
        isAlive = false;
        std::cout << name << " has died." << std::endl;
    }
}

int Player::getX() const { return x; }
int Player::getY() const { return y; }
std::string Player::getName() const { return name; }

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player: " << player.name << "; Position: (" << player.x << ", "
       << player.y << "); Alive: " << (player.isAlive ? "Yes" : "No");
    return os;
}

