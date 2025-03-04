#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <vector>
#include <random>

class NeuralNetwork
{
public:
    NeuralNetwork(int inputNodes, std::vector<int> hiddenLayers, int outputNodes);

    std::vector<double> predict(const std::vector<double> &inputs);
    void mutate(double rate);
    NeuralNetwork crossover(const NeuralNetwork &partner) const;

private:
    int inputNodes;
    std::vector<int> hiddenLayers;
    int outputNodes;

    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> biases;

    double sigmoid(double x) const;
    double randomWeight();
};

#endif // NEURAL_NETWORK_HPP
