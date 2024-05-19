#include "Map.h"

Map::Map() = default;
Map::Map(const Map& other) : layout(other.layout) {}

Map& Map::operator=(const Map& other) {
    if (this != &other) {
        layout = other.layout;
    }
    return *this;
}

Map::~Map() {
    layout.clear();
    std::cout << "Map destroyed\n";
}

std::istream& operator>>(std::istream& is, Map& map) {
    map.layout.clear();
    std::string line;
    while (std::getline(is, line)) {
        if (!line.empty()) {
            map.layout.push_back(line);
        }
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Map& map) {
    for (const auto& line : map.layout) {
        os << line << "\n";
    }
    return os;
}

void Map::updateCell(int x, int y, char symbol) {
    layout[y][x] = symbol;
}

