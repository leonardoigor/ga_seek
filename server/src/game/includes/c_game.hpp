#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include "Agent.hpp"
#include <thread>
#include <chrono>
#include "Obstacle.hpp"

class Game
{
public:
    Game();

    void update();
    void draw();
    void drawNeuralNetwork(const std::vector<float> &inputs, const std::vector<float> &hiddenActivations);

private:
    int populationSize;
    std::vector<Agent> agents;
    std::vector<Obstacle> obstacles;
    int generation;
    Agent *bestAgent;
    float lastOutput;
};

#endif // GAME_HPP
