#include "NeuralNetwork.h"
#include <stdexcept>

NeuralNetwork::NeuralNetwork(const std::vector<int>& layerSizes,
                             std::function<double(double)> activation,
                             std::function<double(double)> activationDerivative) {
    if(layerSizes.size() < 2)
        throw std::runtime_error("NeuralNetwork requires at least input and output layers");

    for(size_t i = 1; i < layerSizes.size(); ++i) {
        layers.emplace_back(Layer(layerSizes[i-1], layerSizes[i], activation, activationDerivative));
    }
}

Matrix NeuralNetwork::forward(const Matrix& input) {
    Matrix out = input;
    for(auto& layer : layers)
        out = layer.forward(out);
    return out;
}

void NeuralNetwork::backward(const Matrix& expected, double learningRate) {
    // Compute gradient at output layer: dC/dA = A - Y
    Matrix dC_dA = layers.back().getOutput() - expected;

    // Backpropagate through layers in reverse
    for(int i = static_cast<int>(layers.size()) - 1; i >= 0; --i) {
        dC_dA = layers[i].backward(dC_dA, learningRate);
    }
}

void NeuralNetwork::train(const Matrix& input, const Matrix& expected, double learningRate) {
    forward(input);
    backward(expected, learningRate);
}

Matrix NeuralNetwork::predict(const Matrix& input) {
    return forward(input);
}

void NeuralNetwork::save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if(!out) throw std::runtime_error("Failed to open file for saving network");

    for(const auto& layer : layers)
        layer.save(out);
}

void NeuralNetwork::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if(!in) throw std::runtime_error("Failed to open file for loading network");

    for(auto& layer : layers)
        layer.load(in);
}