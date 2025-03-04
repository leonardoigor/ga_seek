#ifndef AGENT_HPP
#define AGENT_HPP

#include <sstream>
#include "Obstacle.hpp"
#include "NeuralNetwork.hpp" // Assuming you have a NeuralNetwork class

class Agent
{
public:
    Agent(NeuralNetwork *brain = nullptr);

    void think(const std::vector<Obstacle> &obstacles);
    void jump();
    void duck();
    void update();
    bool collides(const Obstacle &obstacle) const;

    float getY() const { return y; }
    float getVelocity() const { return velocity; }
    int getScore() const { return score; }
    bool getAlive() const { return alive; }

private:
    float y;
    float velocity;
    float gravity;
    bool alive;
    int score;
    NeuralNetwork *brain;
    std::string color;
};

#endif // AGENT_HPP
