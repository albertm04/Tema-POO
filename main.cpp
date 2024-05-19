#include <iostream>
#include <fstream>
#include "Bomb.h"
#include "Game.h"
#include "InvalidMoveException.h"
#include "Map.h"
#include "Player.h"
#include "Weapon.h"

int main() {
    Game game;

    // Attempt to load the map from a file
    try {
        std::ifstream file("map.txt");
        if (!file) {
            throw std::runtime_error("Unable to open file map.txt - Map file cannot be found or opened.");
        }
        game.setMap(file);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;  // Exit if the map cannot be loaded
    }

    // Add players
    Player player1("Alex", 1, 1, game.getMap());  // Start position (1,1)
    Player player2("Bella", 8, 1, game.getMap()); // Start position (8,1)
    game.addPlayer(player1);
    game.addPlayer(player2);

    std::cout << "Initial game state:" << std::endl;
    std::cout << game << std::endl;

    std::cout << "Players are moving..." << std::endl;
    // Simulate player moves
    game.getPlayers()[0].move(1, 0); // Player1 moves to the right
    game.getPlayers()[1].move(-1, 0); // Player2 moves to the left

    std::cout << "Current game state after movements:" << std::endl;
    std::cout << game << std::endl;

    // Test bomb placement and explosions
    game.getPlayers()[0].placeBomb(game.getBombs(), 1); // Player1 places a bomb
    game.getPlayers()[0].move(1, 0);  // Move right again
    game.getPlayers()[0].placeBomb(game.getBombs(), 1); // Player1 places another bomb
    game.explodeAllBombs(); // Simulate exploding all bombs

    std::cout << "Final game state after explosions:" << std::endl;
    std::cout << game << std::endl;

    return 0;
}
