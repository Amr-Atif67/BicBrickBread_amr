#include "../Include/NeuralNetwork.h"
#include <stdexcept>
#include <fstream>

// Constructor: initialize network layers with provided sizes and activations
NeuralNetwork::NeuralNetwork(
    const std::vector<int>& layerSizes,
    const std::vector<std::function<double(double)>>& activations,
    const std::vector<std::function<double(double)>>& activationDerivatives)
{
    if (layerSizes.size() < 2 || activations.size() != layerSizes.size() - 1 || activationDerivatives.size() != layerSizes.size() - 1)
        throw std::runtime_error("NeuralNetwork constructor: size mismatch");

    layers_.clear();
    for (size_t i = 1; i < layerSizes.size(); ++i) {
        layers_.emplace_back(layerSizes[i-1], layerSizes[i], activations[i-1], activationDerivatives[i-1]);
    }
}

// Forward pass: propagate input through all layers
Matrix<double> NeuralNetwork::forward(const Matrix<double>& input) {
    Matrix<double> output = input;
    for (auto& layer : layers_) {
        output = layer.forward(output);
    }
    return output;
}

// Backward pass: propagate gradients and update weights
void NeuralNetwork::backward(const Matrix<double>& expected, double learningRate) {
    if (layers_.empty()) return;

    // Compute gradient for the last layer
    Matrix<double> dC_dA = layers_.back().output() - expected;

    // Backpropagate through layers in reverse order
    for (auto it = layers_.rbegin(); it != layers_.rend(); ++it) {
        dC_dA = it->backward(dC_dA, learningRate);
    }
}

// Train network on a single input-output pair
void NeuralNetwork::train(const Matrix<double>& input, const Matrix<double>& expected, double learningRate) {
    forward(input);
    backward(expected, learningRate);
}

// Predict output without modifying weights
Matrix<double> NeuralNetwork::predict(const Matrix<double>& input) {
    return forward(input);
}

// Save network layers to a binary file
void NeuralNetwork::save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) throw std::runtime_error("Failed to open file for saving NeuralNetwork");

    for (const auto& layer : layers_) {
        layer.save(out);
    }
}

// Load network layers from a binary file
void NeuralNetwork::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) throw std::runtime_error("Failed to open file for loading NeuralNetwork");

    for (auto& layer : layers_) {
        layer.load(in);
    }
}