#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

class Map {
private:
    std::vector<std::string> layout;
    // Legenda harta:
    // # = zid
    // . = zona libera
    // A = player A
    // B = player B
    // x = bomba plantata
    // X = bomba care a explodat
public:
    
    Map() = default;


    friend std::istream& operator>>(std::istream& is, Map& map) {
        map.layout.clear();
        std::string line;
        while (std::getline(is, line)) {

            if (!line.empty()) {
                map.layout.push_back(line);
            }
        }
        return is;
    }


    Map(const Map& other) : layout(other.layout) {}


    Map& operator=(const Map& other) {
        if (this != &other) {
            layout = other.layout;
        }
        return *this;
    }


    ~Map() {
        layout.clear();
        std::cout << "Map 1 - Completed; Next map: loading..." << std::endl;
    }


    const std::vector<std::string>& getLayout() const {
        return layout;
    }


    void updateCell(int x, int y, char symbol) {
            layout[y][x] = symbol;
    }


    friend std::ostream& operator<<(std::ostream& os, const Map& map) {
        for (const auto& line : map.layout) {
            os << line << "\n";
        }
        return os;
    }

};

class Bomb {
private:
    int x, y; // pozitia bombei pe harta, in sistemul de coordonate xOy
    int explosionRange; // distanta pe care bomba explodeaza (sus, jos, stanga, dreapta)
    static void atention(){std::cout<<"Atentie, a fost plasata o bomba!!!" << std::endl;}
public:
    // Constructor
    Bomb(int x, int y, int explosionRange)
            : x(x), y(y), explosionRange(explosionRange) {
        atention();
    }

    // Getteri si Setteri
    int getX() const { return x; }
    int getY() const { return y; }
    int getExplosionRange() const { return explosionRange; }
    void setExplosionRange(int newRange) { explosionRange = newRange; }


    bool isInRange(int targetX, int targetY) const {
        return abs(targetX - x) <= explosionRange && abs(targetY - y) <= explosionRange;
    }


    friend std::ostream& operator<<(std::ostream& os, const Bomb& bomb) {
        os << "Bomb at (" << bomb.x << ", " << bomb.y << ") with range " << bomb.explosionRange;
        return os;
    }
};

class Player {
private:
    std::string name;
    int x, y;
    bool isAlive;
    Map& map;
public:

    Player(const std::string& name, int startX, int startY, Map& map)
            : name(name), x(startX), y(startY), isAlive(true), map(map) {}


    int getX() const { return x; }
    int getY() const { return y; }
    std::string getName() const {return name;}

    // Plasare bomba
    void placeBomb(std::vector<Bomb>& bombs, int explosionRange) {
        bombs.push_back(Bomb(x, y, explosionRange));
        map.updateCell(x,y,'x');
    }


    void die() {
        if(isAlive) {
            isAlive = false;
            std::cout << name << " has died." << std::endl << std::endl;
        }
    }


    void move(int deltaX, int deltaY) {
        int newX = x + deltaX;
        int newY = y + deltaY;
        map.updateCell(newX, newY, name[0]);
        if(map.getLayout()[y][x]!='x')
        {map.updateCell(x, y, '.');}
        x = newX;
        y = newY;
    }


    friend std::ostream& operator<<(std::ostream& os, const Player& player) {
        os << "Player: " << player.name << "; Position: (" << player.x << ", "
        << player.y << "); Alive: " << (player.isAlive ? "Yes" : "No");
        return os;
    }


};

class Game {
private:
    std::unique_ptr<Map> gameMap;
    std::vector<Player> players;
    std::vector<Bomb> bombs;
public:


    Game() : gameMap(std::make_unique<Map>()){}


    void setMap(std::istream& is) {
        is >> *gameMap;
    }


    Map& getMap() {
        return *gameMap;
    }


    void addPlayer(const Player& player) {
        players.push_back(player);
        gameMap->updateCell(player.getX(), player.getY(), player.getName()[0]);
    }


    std::vector<Player>& getPlayers()  {
        return players;
    }

    std::vector<Bomb>& getBombs() {
        return bombs;
    }


    void explodeAllBombs() {
        for (auto& bomb : bombs) {
            gameMap->updateCell(bomb.getX(), bomb.getY(),'X');
            for (auto& player : players) {
                if (bomb.isInRange(player.getX(), player.getY())) {
                    player.die();
                }
            }
        }
        bombs.clear();
    }

    friend std::ostream& operator<<(std::ostream& os, const Game& game) {
        os << *(game.gameMap); // Afiseaza harta
        for (const auto& player : game.players) {
            os << player << "\n";
        }
        for (const auto& bomb : game.bombs) {
            os << bomb << "\n";
        }
        return os;
    }
};

void test_verif_jucatori(Game& game){

    assert(game.getPlayers().size() == 2);
    assert(game.getPlayers()[0].getX() == 1 && game.getPlayers()[0].getY() == 1);
    assert(game.getPlayers()[1].getX() == 8 && game.getPlayers()[1].getY() == 1);
    std::cout << "Jucatorii au fost adaugati cu succes in joc!" << std::endl;
}

void test_deplasare_jucatori(Game &game){

    assert(game.getPlayers()[0].getX() == 2 && game.getPlayers()[0].getY() == 1);
    assert(game.getPlayers()[1].getX() == 7 && game.getPlayers()[1].getY() == 1);
    std::cout << "Testele de verificare ale pozițiilor au trecut cu succes." << std::endl;
}

void test_bombe(Game &game){

    game.getPlayers()[0].placeBomb(game.getBombs(), 1);
    assert(game.getBombs().size() == 1);
    std::cout << "Starea jocului dupa ce Player1 plaseaza o bomba:" << std::endl;
    std::cout << game << std::endl;


    game.getPlayers()[0].move(1, 0);
    std::cout << "Starea jocului dupa ce Player1 se muta la dreapta:" << std::endl << game << std::endl;


    game.getPlayers()[0].placeBomb(game.getBombs(), 1);
    assert(game.getBombs().size() == 2);
    std::cout << "Starea jocului dupa ce Player1 mai plaseaza o bomba:" << std::endl << game << std::endl;


    std::cout << "All bombs have exploded" << std::endl;
    game.explodeAllBombs();
    assert(game.getBombs().empty());
    std::cout <<"Bombele au fost plasate si au explodat cu succes." << std::endl;
}

int main() {

    Game game;

    std::ifstream file("map.txt");
    if (file)
        game.setMap(file);
    else {
        std::cerr << "Unable to open file map.txt" << std::endl;
        return 1;
    }

    // Adaugam 2 jucatori
    Player player1("Dorel", 1, 1, game.getMap()); // Pozitie de inceput (1,1)
    Player player2("Mirabela", 8, 1, game.getMap());   // Pozitie de inceput (8,1)
    game.addPlayer(player1);
    game.addPlayer(player2);

    test_verif_jucatori(game);
    std::cout << "Starea initiala a jocului:" << std::endl << game << std::endl;

    std::cout<<"Jucatorii se deplaseaza cu o pozitie spre dreapta, respectiv spre stanga." <<std::endl;
    game.getPlayers()[0].move(1, 0); // Player1 se muta la dreapta
    game.getPlayers()[1].move(-1, 0); // Player2 se muta la stanga

    test_deplasare_jucatori(game);
    std::cout << "Starea curenta a jocului:" << std::endl << game << std::endl;

    test_bombe(game);
    std::cout << "Starea jocului dupa explozie:" << std::endl << game << std::endl;

    return 0;
}