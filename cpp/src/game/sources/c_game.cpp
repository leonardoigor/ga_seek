#include "c_game.hpp"
#include <iostream>
#include <sstream> // For stringstream (if needed for string formatting)

Game::Game()
    : populationSize(500), generation(1), bestAgent(nullptr), lastOutput(0)
{

    // Initialize agents and obstacles
    for (int i = 0; i < populationSize; ++i)
    {
        agents.push_back(Agent());
    }
    obstacles.push_back(Obstacle());

    // Set interval to increase speed every 5 seconds
    // In C++, you can use a separate thread to handle periodic actions like this
    std::thread speedIncrease([this]()
                              {
        while (true) {
            if (Obstacle::speed < Obstacle::MaxSpeed) {
                Obstacle::speed++;
            }
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait 5 seconds before increasing speed
        } });
    speedIncrease.detach(); // Detach the thread so it runs asynchronously
}

void Game::update()
{
    draw();
}

void Game::drawNeuralNetwork(const std::vector<float> &inputs, const std::vector<float> &hiddenActivations)
{
    // Implement drawing of the neural network here
    // (This part will depend on how you want to visualize the neural network)
}

void Game::draw()
{
    // In C++, we do not have a native canvas context like in HTML5, so this would be done in a graphical library like SFML or SDL.
    // For simplicity, just printing to console for now.

    // Clear the screen (in console, just print new lines for simplicity)
    std::cout << "\033[H\033[J"; // Clear console screen

    // Draw the status (example)
    std::cout << "Generation: " << generation << std::endl;
    std::cout << "Speed:      " << Obstacle::speed << std::endl;
    std::cout << "Alive:      " << std::count_if(agents.begin(), agents.end(), [](const Agent &a)
                                                 { return a.getAlive(); })
              << std::endl;
    std::cout << "Agents:     " << (obstacles[0].getIsBird() ? "Bird" : "Other") << std::endl;

    // Simulate drawing the horizontal line for the "ground"
    std::cout << "Ground line at Y = 360" << std::endl;
}
