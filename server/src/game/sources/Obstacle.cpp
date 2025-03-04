#include "Obstacle.hpp"
#include <cstdlib> // For rand()
#include <ctime>   // For time()

// Initialize static variables
int Obstacle::speed = 5;
int Obstacle::MaxSpeed = 20;

Obstacle::Obstacle(bool isBird)
{
    x = 800;
    width = 20;
    height = isBird ? 20 : rand() % 50 + 20; // Random height between 20 and 70
    this->isBird = isBird;

    // Uncomment to set a random speed
    // speed = rand() % 2 == 0 ? 10 : 5;
}

void Obstacle::update()
{
    x -= speed;
}
