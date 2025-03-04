#include "Agent.hpp"
#include <cstdlib>

Agent::Agent(NeuralNetwork *brain)
    : y(150), velocity(0), gravity(0.6), alive(true), score(0),
      brain(brain ? brain : new NeuralNetwork(6, {6, 6}, 2))
{

    // Generate random color
    color = "rgb(" + std::to_string(rand() % 256) + ", " + std::to_string(rand() % 256) + ", " + std::to_string(rand() % 256) + ")";
}

void Agent::think(const std::vector<Obstacle> &obstacles)
{
    // Set inputs for the neural network
    std::vector<double> inputs = {
        static_cast<double>(y) / 150.0f,                                                   // Normalize by max Dino height (assuming 150 is ground level)
        static_cast<double>((obstacles.empty() ? 800 : obstacles[0].getX())) / 800.0f,     // Normalize by max screen width
        static_cast<double>((obstacles.empty() ? 20 : obstacles[0].getWidth())) / 100.0f,  // Normalize width
        static_cast<double>((obstacles.empty() ? 50 : obstacles[0].getHeight())) / 100.0f, // Normalize height
        static_cast<double>(Obstacle::speed) / Obstacle::MaxSpeed,                         // Normalized speed
        1.0f - (static_cast<double>(Obstacle::speed) / Obstacle::MaxSpeed)                 // Alternative representation of speed scaling
    };

    // Get output from the neural network
    auto outputs = brain->predict(inputs);

    if (outputs[0] > 0.5)
        jump();
    if (outputs[1] > 0.5)
        duck();
}

void Agent::jump()
{
    if (y == 150)
    {
        velocity = -10;
    }
}

void Agent::duck()
{
    y += 10; // Duck by moving the Y position down
}

void Agent::update()
{
    velocity += gravity;
    y += velocity;
    if (y > 150)
        y = 150;
    score++;
}

bool Agent::collides(const Obstacle &obstacle) const
{
    return y >= 150 - obstacle.getHeight() && obstacle.getX() < 70 && obstacle.getX() + obstacle.getWidth() > 50;
}
