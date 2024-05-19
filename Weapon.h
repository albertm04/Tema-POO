#ifndef WEAPON_H
#define WEAPON_H

class Weapon {
protected:
    virtual bool isInRange(int targetX, int targetY) = 0;
};

#endif // WEAPON_H
