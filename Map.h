#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <iostream>

class Map {
private:
    std::vector<std::string> layout;

public:
    Map();
    Map(const Map& other);
    Map& operator=(const Map& other);
    ~Map();

    friend std::istream& operator>>(std::istream& is, Map& map);
    friend std::ostream& operator<<(std::ostream& os, const Map& map);
    const std::vector<std::string>& getLayout() const;
    void updateCell(int x, int y, char symbol);
};

#endif // MAP_H
