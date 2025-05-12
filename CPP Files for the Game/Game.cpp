# include "Game.h"
#include "Level.h"
using namespace sf;

Game::Game() {};

void Game::run()
{
    sf::RenderWindow window(sf::VideoMode(1360, 750), "Game");
    window.setFramerateLimit(60);

    // Initialize the first level
    Level* current = new Home(&window);

    while (window.isOpen()) {

        current->processEvents(); // Handle events (key presses, window events)
        current->update(); // Update the game state (avatar movement, timer, etc.)
        current->render(); // Render the game to the window

        // Check if the current level is finished and elevate to the next level
        if (current->isFinished()) {
            
            Level* next = current->createNext(&window);
            delete current;
            if (!next) {
                window.close();     // or break;
                break;
            }
            current = next;
        }
    }
}
Game::~Game() {}