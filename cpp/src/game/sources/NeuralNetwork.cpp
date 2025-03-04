#include "NeuralNetwork.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <ctime>

// Construtor
NeuralNetwork::NeuralNetwork(int inputNodes, std::vector<int> hiddenLayers, int outputNodes)
    : inputNodes(inputNodes), hiddenLayers(hiddenLayers), outputNodes(outputNodes)
{

    std::srand(std::time(0)); // Seed para valores aleatórios

    // Criar pesos entre input -> primeira hidden
    weights.push_back(std::vector<std::vector<double>>(hiddenLayers[0], std::vector<double>(inputNodes)));
    biases.push_back(std::vector<double>(hiddenLayers[0]));

    // Criar pesos entre camadas ocultas
    for (size_t i = 0; i < hiddenLayers.size() - 1; i++)
    {
        weights.push_back(std::vector<std::vector<double>>(hiddenLayers[i + 1], std::vector<double>(hiddenLayers[i])));
        biases.push_back(std::vector<double>(hiddenLayers[i + 1]));
    }

    // Criar pesos entre última hidden -> output
    weights.push_back(std::vector<std::vector<double>>(outputNodes, std::vector<double>(hiddenLayers.back())));
    biases.push_back(std::vector<double>(outputNodes));

    // Inicializar pesos e biases com valores aleatórios entre -1 e 1
    for (auto &layer : weights)
    {
        for (auto &neuron : layer)
        {
            for (auto &w : neuron)
            {
                w = randomWeight();
            }
        }
    }

    for (auto &layer : biases)
    {
        for (auto &b : layer)
        {
            b = randomWeight();
        }
    }
}

// Função de ativação Sigmoid
double NeuralNetwork::sigmoid(double x) const
{
    return 1.0 / (1.0 + std::exp(-x));
}

// Gera um número aleatório entre -1 e 1
double NeuralNetwork::randomWeight()
{
    return ((double)std::rand() / RAND_MAX) * 2.0 - 1.0;
}

// Método de previsão (feedforward)
std::vector<double> NeuralNetwork::predict(const std::vector<double> &inputs)
{
    std::vector<double> activations = inputs;

    for (size_t layerIdx = 0; layerIdx < weights.size(); layerIdx++)
    {
        std::vector<double> nextActivations(weights[layerIdx].size(), 0.0);

        for (size_t neuronIdx = 0; neuronIdx < weights[layerIdx].size(); neuronIdx++)
        {
            double sum = biases[layerIdx][neuronIdx];

            for (size_t inputIdx = 0; inputIdx < activations.size(); inputIdx++)
            {
                sum += weights[layerIdx][neuronIdx][inputIdx] * activations[inputIdx];
            }

            nextActivations[neuronIdx] = sigmoid(sum);
        }

        activations = nextActivations;
    }

    return activations;
}

// Mutação dos pesos e biases
void NeuralNetwork::mutate(double rate)
{
    for (auto &layer : weights)
    {
        for (auto &neuron : layer)
        {
            for (auto &w : neuron)
            {
                if ((double)std::rand() / RAND_MAX < rate)
                {
                    w += ((double)std::rand() / RAND_MAX) * 0.2 - 0.1;
                }
            }
        }
    }

    for (auto &layer : biases)
    {
        for (auto &b : layer)
        {
            if ((double)std::rand() / RAND_MAX < rate)
            {
                b += ((double)std::rand() / RAND_MAX) * 0.2 - 0.1;
            }
        }
    }
}

// Crossover entre duas redes neurais
NeuralNetwork NeuralNetwork::crossover(const NeuralNetwork &partner) const
{
    NeuralNetwork offspring(inputNodes, hiddenLayers, outputNodes);

    for (size_t layerIdx = 0; layerIdx < weights.size(); layerIdx++)
    {
        for (size_t neuronIdx = 0; neuronIdx < weights[layerIdx].size(); neuronIdx++)
        {
            for (size_t wIdx = 0; wIdx < weights[layerIdx][neuronIdx].size(); wIdx++)
            {
                offspring.weights[layerIdx][neuronIdx][wIdx] =
                    (std::rand() % 2 == 0) ? weights[layerIdx][neuronIdx][wIdx]
                                           : partner.weights[layerIdx][neuronIdx][wIdx];
            }
        }
    }

    for (size_t layerIdx = 0; layerIdx < biases.size(); layerIdx++)
    {
        for (size_t neuronIdx = 0; neuronIdx < biases[layerIdx].size(); neuronIdx++)
        {
            offspring.biases[layerIdx][neuronIdx] =
                (std::rand() % 2 == 0) ? biases[layerIdx][neuronIdx]
                                       : partner.biases[layerIdx][neuronIdx];
        }
    }

    return offspring;
}
