#include "NeuralNetwork.h"
#include <fstream>
#include <stdexcept>

/**
 * @brief Construct a feedforward neural network
 * @param layerSizes Vector of layer sizes (input to output)
 * @param activation Activation function for all layers
 * @param activationDerivative Derivative of activation
 */
NeuralNetwork::NeuralNetwork(const std::vector<int>& layerSizes,
                            const std::vector<std::function<double(double)>>& activations,
                            const std::vector<std::function<double(double)>>& activationDerivatives)
{
    if (layerSizes.size() < 2)
        throw std::runtime_error("NeuralNetwork: need at least input and output layers");

    if (activations.size() != layerSizes.size() - 1 || activationDerivatives.size() != layerSizes.size() - 1)
        throw std::runtime_error("Activations size mismatch with layerSizes");

    for (size_t i = 1; i < layerSizes.size(); i++) {
        layers.emplace_back(layerSizes[i-1], layerSizes[i], activations[i-1], activationDerivatives[i-1]);
    }
}

/**
 * @brief Forward pass through all layers
 * @param input Input matrix (inputSize x 1)
 * @return Output of last layer
 */
Matrix NeuralNetwork::forward(const Matrix& input){
    Matrix current = input;
    for(auto& layer : layers){
        current = layer.forward(current);
    }
    return current;
}

/**
 * @brief Backpropagation using Mean Squared Error
 * @param expected Target output (same shape as last layer)
 * @param learningRate Learning rate
 */
void NeuralNetwork::backward(const Matrix& expected, double learningRate){
    if(layers.empty()) return;

    // Compute gradient at output: dC/dA = A - expected
    Matrix dC_dA = layers.back().A - expected;

    // Backpropagate through layers in reverse
    for(int i=int(layers.size())-1; i>=0; i--){
        dC_dA = layers[i].backward(dC_dA, learningRate);
    }
}

/**
 * @brief Train on a single example
 * @param input Input matrix
 * @param expected Target output
 * @param learningRate Learning rate
 */
void NeuralNetwork::train(const Matrix& input, const Matrix& expected, double learningRate){
    forward(input);
    backward(expected, learningRate);
}

/**
 * @brief Predict output for given input
 * @param input Input matrix
 * @return Output of network
 */
Matrix NeuralNetwork::predict(const Matrix& input){
    return forward(input);
}

/**
 * @brief Save network to binary file
 * @param filename File path
 */
void NeuralNetwork::save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if(!out) throw std::runtime_error("Failed to open file for saving NN");
    for(const auto& layer : layers) layer.save(out);
}

/**
 * @brief Load network from binary file
 * @param filename File path
 */
void NeuralNetwork::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
        throw std::runtime_error("Error: Cannot open file '" + filename + "'");

    // ----------------------------------------------------
    // Compute EXACT expected byte size
    // ----------------------------------------------------
    size_t expectedBytes = 0;
    for (auto& layer : layers) {
        expectedBytes += layer.W.rows * layer.W.cols * sizeof(double);
        expectedBytes += layer.B.rows * sizeof(double);
    }

    // ----------------------------------------------------
    // Get actual file size
    // ----------------------------------------------------
    in.seekg(0, std::ios::end);
    size_t fileSize = static_cast<size_t>(in.tellg());
    in.seekg(0, std::ios::beg);

    // ----------------------------------------------------
    // Strict size validation
    // ----------------------------------------------------
    if (fileSize < expectedBytes) {
        throw std::runtime_error(
            "Error: file '" + filename + "' is TOO SMALL.\nExpected " +
            std::to_string(expectedBytes) + " bytes, got " +
            std::to_string(fileSize) + " bytes.");
    }

    if (fileSize > expectedBytes) {
        throw std::runtime_error(
            "Error: file '" + filename + "' is TOO LARGE.\nExpected " +
            std::to_string(expectedBytes) + " bytes, got " +
            std::to_string(fileSize) + " bytes.");
    }

    // ----------------------------------------------------
    // Load weights & biases
    // ----------------------------------------------------
    for (auto& layer : layers) {
        layer.load(in);
        if (!in.good())
            throw std::runtime_error("Error: corrupted or truncated file while loading '" + filename + "'");
    }
}