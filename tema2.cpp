#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <exception>

// exceptie custom pentru deplasare invalida
class InvalidMoveException : public std::exception {
private:
    std::string message;
public:
    InvalidMoveException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class Map {
private:
    std::vector<std::string> layout;
    // salvam harta in memorie ca un vector de string-uri:
    // Legenda harta:
    // # = zid
    // . = zona libera
    // A = player A
    // B = player B
    // x = bomba plantata
    // X = bomba care a explodat

public:
    // Constructor implicit
    Map() = default;

    // Suprascrierea operatorului >>
    friend std::istream& operator>>(std::istream& is, Map& map) {
        map.layout.clear(); // Curăță harta curentă înainte de a citi una nouă
        std::string line;
        while (std::getline(is, line)) {
            // Presupunând că vrei să omiti liniile goale
            if (!line.empty()) {
                map.layout.push_back(line);
            }
        }
        return is;
    }

    // Constructorul de copiere - creaza o noua harta copie a hartii initiale
    Map(const Map& other) : layout(other.layout) {}

    // operator= de atribuire - copiaza starea din harta curenta intr-o alta harta
    Map& operator=(const Map& other) {
        if (this != &other) { // Protecție împotriva auto-atribuirii
            layout = other.layout;
        }
        return *this;
    }

    // Destructor - sterge harta curenta din memorie, de ex. cand trecem la nivelul urmator
    ~Map() {
        layout.clear();
        std::cout << "Map 1 - Completed; Next map: loading..." << std::endl;
    }

    // Getter pentru layout (harta sub forma de string)
    const std::vector<std::string>& getLayout() const {
        return layout;
    }

    // Actualizează o patratica de pe harta
    void updateCell(int x, int y, char symbol) {
            layout[y][x] = symbol;
    }

    // supraincarcam operator<< pentru afișare (in loc de functia display de mai jos)
    friend std::ostream& operator<<(std::ostream& os, const Map& map) {
        for (const auto& line : map.layout) {
            os << line << "\n";
        }
        return os;
    }
};

// clasa abstracta cu o singura metoda virtuala
class Weapon {
protected:
    virtual bool isInRange(int targetX, int targetY) = 0;
};

class Bomb :  Weapon {
private:
    int x, y; // pozitia bombei pe harta, in sistemul de coordonate xOy
    int explosionRange; // distanta pe care bomba explodeaza (sus, jos, stanga, dreapta)
    static int nrOfBombs; // bombe neexplodate
    static void atention(){std::cout<<"Atentie, a fost plasata o bomba!!! Total bombe neexplodate: " << std::to_string(nrOfBombs) << std::endl;}
public:
    static void decNrOfBombs(){nrOfBombs--; std::cout<<"Atentie, o bomba a explodat! Total bombe neexplodate: " << std::to_string(nrOfBombs)<< std::endl;}

    // Constructor
    Bomb(int x, int y, int explosionRange)
            : x(x), y(y), explosionRange(explosionRange) {
        if (explosionRange < 1) {
            throw std::invalid_argument("Explosion range must be at least 1.");
        }
        nrOfBombs++;
        atention();
    }

    // Getteri si Setteri
    int getX() const { return x; }
    int getY() const { return y; }
    int getExplosionRange() const { return explosionRange; }
    void setExplosionRange(int newRange) { explosionRange = newRange; }

    // verifica daca target-ul este in explosionRange
    bool isInRange(int targetX, int targetY) override {
        return abs(targetX - x) <= explosionRange && abs(targetY - y) <= explosionRange;
    }

    // afiseaza locatia bombei folosind operatorul<< supraincarcat
    friend std::ostream& operator<<(std::ostream& os, const Bomb& bomb) {
        os << "Bomb at (" << bomb.x << ", " << bomb.y << ") with range " << bomb.explosionRange;
        return os;
    }
};

class Player {
private:
    int x, y; // pozitia jucatorului pe orizontala si verticala
    // starea jucatorului
    Map& map;
protected:
    std::string name;
    bool isAlive;
public:
    // Constructor
    Player(const std::string& name, int startX, int startY, Map& map)
            : name(name), x(startX), y(startY), isAlive(true), map(map) {}

    // Getteri pentru pozitie
    int getX() const { return x; }
    int getY() const { return y; }
    std::string getName() const {return name;}

    // Plasare bomba (= adaugarea bombei curente pusa de jucator in vectorul de bombe)
    void placeBomb(std::vector<Bomb>& bombs, int explosionRange) {
        bombs.push_back(Bomb(x, y, explosionRange));
        map.updateCell(x,y,'x');
    }

    // Jucatorul este distrus de bomba
    void die() {
        if(isAlive) {
            isAlive = false;
            std::cout << name << " has died." << std::endl << std::endl;
        }
    }

    virtual // Jucatorul se deplaseaza cu deltaX (pe orizontala) si deltaY (pe verticala)
    void move(int deltaX, int deltaY) {
        int newX = x + deltaX;
        int newY = y + deltaY;

        // Verificam daca noua pozitie este in limitele hartii, altfel aruncam eroare
        if (newX < 0 || newX > map.getLayout().size() || newY < 0 || newY > map.getLayout()[0].length()) {
            throw InvalidMoveException("Move out of map bounds!");
        }

        map.updateCell(newX, newY, name[0]); // Actualizăm harta cu noua poziție
        if(map.getLayout()[y][x]!='x')
        {map.updateCell(x, y, '.');} // Marchează vechea poziție ca libera
        x = newX;
        y = newY;
    }

    // afiseaza locatia si starea jucatorului folosind operatorul<< supraincarcat
    friend std::ostream& operator<<(std::ostream& os, const Player& player) {
        os << "Player: " << player.name << "; Position: (" << player.x << ", "
        << player.y << "); Alive: " << (player.isAlive ? "Yes" : "No");
        return os;
    }

};

// un nou tip de jucator: invizibil
class StealthPlayer : public Player {
protected:
    bool isInvisible;

public:
    StealthPlayer(const std::string& name, int startX, int startY, Map& map)
            : Player(name, startX, startY, map), isInvisible(false) {}

    void goInvisible() {
        isInvisible = true;
        std::cout << name << " is now invisible!" << std::endl;
    }

    virtual void move(int deltaX, int deltaY) override {
        if (isInvisible) {
            std::cout << name << " moves stealthily." << std::endl;
        }
        Player::move(deltaX, deltaY); // Call base class method
    }
};

// un nou tip de jucator: turbo
class SpeedyPlayer : public Player {
protected:
    int speedMultiplier; // Multiplicator de viteza

public:
    // Constructor pentru SpeedyPlayer care foloseste constructorul clasei Player
    SpeedyPlayer(const std::string& name, int startX, int startY, Map& map, int multiplier = 2)
            : Player(name, startX, startY, map), speedMultiplier(multiplier) {}

    // Metoda move din clasa player, suprascrisa in clasa SpeedyPlayer
    void move(int deltaX, int deltaY) override {
        Player::move(deltaX * speedMultiplier, deltaY * speedMultiplier);
    }

};

class Game {
private:
    std::unique_ptr<Map> gameMap; // incarca harta curenta
    std::vector<Player> players; // vectorul de jucatori
    std::vector<Bomb> bombs; // vectorul cu toate bombele neexplodate din joc
public:

    // Constructor implicit
    Game() : gameMap(std::make_unique<Map>()){}

    // Setter pentru harta jocului
    void setMap(std::istream& is) {
        is >> *gameMap;
    }

    // Getter pentru harta jocului
    Map& getMap() {
        return *gameMap;
    }

    // adauga un jucator nou
    void addPlayer(const Player& player) {
        players.push_back(player);
        gameMap->updateCell(player.getX(), player.getY(), player.getName()[0]);
    }

    // Getter pentru jucători
    std::vector<Player>& getPlayers()  {
        return players;
    }

    // Getter pentru bombe
    std::vector<Bomb>& getBombs() {
        return bombs;
    }

    // simuleaza explozia tuturor bombelor in acelasi timp
    void explodeAllBombs() {
        for (auto& bomb : bombs) {
            Bomb::decNrOfBombs();
            gameMap->updateCell(bomb.getX(), bomb.getY(),'X');
            for (auto& player : players) {
                if (bomb.isInRange(player.getX(), player.getY())) {
                    // Polimorfism la execuție: Apelul se face către isInRange() din Bomb sau alte clase derivate
                    // Upcasting: player este tratat ca referință la clasa de bază Player
                    player.die(); // jucatorii aflati inRange vor muri
                }
            }
        }
        bombs.clear(); // elimina bombele care au explodat din vectorul de bombe
    }

    friend std::ostream& operator<<(std::ostream& os, const Game& game) {
        os << *(game.gameMap); // Afiseaza harta
        for (const auto& player : game.players) {
            os << player << "\n"; // Afiseaza fiecare jucător
        }
        for (const auto& bomb : game.bombs) {
            os << bomb << "\n"; // Afiseaza fiecare bomba
        }
        return os;
    }
};

void test_verif_jucatori(Game& game){
    // Verificam daca nr de jucatori si pozitiile lor sunt corecte
    assert(game.getPlayers().size() == 2);
    assert(game.getPlayers()[0].getX() == 1 && game.getPlayers()[0].getY() == 1);
    assert(game.getPlayers()[1].getX() == 8 && game.getPlayers()[1].getY() == 1);
    std::cout << "Jucatorii au fost adaugati cu succes in joc!" << std::endl;
}

void test_deplasare_jucatori(Game &game){
    // Verificăm direct pozițiile finale așteptate
    assert(game.getPlayers()[0].getX() == 2 && game.getPlayers()[0].getY() == 1); // Verificăm poziția finală a Player1
    assert(game.getPlayers()[1].getX() == 7 && game.getPlayers()[1].getY() == 1); // Verificăm poziția finală a Player2
    std::cout << "Testele de verificare ale pozițiilor au trecut cu succes." << std::endl;
}

void test_bombe(Game &game){
    // Player1 plaseaza bomba
    game.getPlayers()[0].placeBomb(game.getBombs(), 1);
    assert(game.getBombs().size() == 1); // verificam nr de bombe
    std::cout << "Starea jocului dupa ce Player1 plaseaza o bomba:" << std::endl << game << std::endl;

    // Player1 se muta la dreapta si mai pune o bomba
    game.getPlayers()[0].move(1, 0);
    std::cout << "Starea jocului dupa ce Player1 se muta la dreapta:" << std::endl << game << std::endl;

    // Player1 plaseaza inca o bomba
    game.getPlayers()[0].placeBomb(game.getBombs(), 1);
    assert(game.getBombs().size() == 2); // verificam nr de bombe
    std::cout << "Starea jocului dupa ce Player1 mai plaseaza o bomba:" << std::endl << game << std::endl;

    // Explodeaza toate bombele
    std::cout << "Toate bombele au explodat" << std::endl;
    game.explodeAllBombs();
    assert(game.getBombs().empty()); // verificam daca au explodat toate bombele
    std::cout <<"Bombele au fost plasate si au explodat cu succes." << std::endl;
}

int Bomb::nrOfBombs = 0;

int main() {

    Game game;

    // Incercam sa incarcam harta din fisier
    try {
        std::ifstream file("map.txt");
        if (!file) {
            throw std::runtime_error("Unable to open file map.txt - Map file cannot be found or opened.");
        }
        game.setMap(file);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;  // intrerupem executia programului daca nu putem incarca harta
    }


    // Adaugam 2 jucatori
    Player player1("Alex", 1, 1, game.getMap()); // Pozitie de inceput (1,1)
    Player player2("Bella", 8, 1, game.getMap());   // Pozitie de inceput (8,1)
    game.addPlayer(player1); // Upcasting: player1 este tratat ca Player, chiar dacă este StealthPlayer sau SpeedyPlayer
    game.addPlayer(player2); // Upcasting: player2 este tratat ca Player, chiar dacă este StealthPlayer sau SpeedyPlayer

    test_verif_jucatori(game);
    std::cout << "Starea initiala a jocului:" << std::endl << game << std::endl;

    std::cout<<"Jucatorii se deplaseaza cu o pozitie spre dreapta, respectiv spre stanga." <<std::endl;

    // Polimorfism la execuție: Apelul se face către move() din Player, StealthPlayer sau SpeedyPlayer, în funcție de tipul obiectului la runtime
    game.getPlayers()[0].move(1, 0); // Player1 se muta la dreapta
    game.getPlayers()[1].move(-1, 0); // Player2 se muta la stanga

    test_deplasare_jucatori(game);
    std::cout << "Starea curenta a jocului:" << std::endl << game << std::endl;

    test_bombe(game);
    std::cout << "Starea jocului dupa explozie:" << std::endl << game << std::endl;

    return 0;
}