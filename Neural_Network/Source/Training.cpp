#include "../Neural_Network/Include/Training.h"
#include <iostream>
#include <stdexcept>

Training::Training(NeuralNetwork& network) 
    : network(network) 
{ }

void Training::trainEpoch(const std::vector<Matrix>& inputs,
                          const std::vector<Matrix>& targets,
                          double learningRate)
{
    if (inputs.size() != targets.size())
        throw std::runtime_error("Inputs and targets size mismatch");

    for (size_t i = 0; i < inputs.size(); ++i) {
        // Forward pass
        Matrix output = network.forward(inputs[i]);

        // Compute gradient of cost and backpropagate
        network.backward(targets[i], learningRate);
    }
}

void Training::train(const std::vector<Matrix>& inputs,
                     const std::vector<Matrix>& targets,
                     double learningRate,
                     int epochs,
                     bool verbose)
{
    for (int e = 0; e < epochs; ++e) {
        trainEpoch(inputs, targets, learningRate);

        if (verbose) {
            // Compute total MSE over dataset
            double totalError = 0.0;
            for (size_t i = 0; i < inputs.size(); ++i) {
                Matrix pred = network.forward(inputs[i]);
                totalError += meanSquaredError(pred, targets[i]);
            }
            totalError /= inputs.size();
            std::cout << "Epoch " << e + 1 << "/" << epochs 
                      << " - MSE: " << totalError << std::endl;
        }
    }
}

double Training::meanSquaredError(const Matrix& predicted, const Matrix& target) const
{
    if (predicted.rows != target.rows || predicted.cols != target.cols)
        throw std::runtime_error("Predicted and target size mismatch");

    double sum = 0.0;
    for (int r = 0; r < predicted.rows; ++r) {
        for (int c = 0; c < predicted.cols; ++c) {
            double diff = predicted(r, c) - target(r, c);
            sum += diff * diff;
        }
    }
    return sum / (predicted.rows * predicted.cols);
}