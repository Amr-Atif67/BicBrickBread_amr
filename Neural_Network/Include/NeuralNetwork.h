#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Matrix.h"
#include "Layer.h"
#include <vector>
#include <functional>
#include <fstream>
#include <string>

/**
 * @class NeuralNetwork
 * @brief Simple fully connected feedforward neural network.
 *
 * Supports forward propagation, backpropagation, training on single
 * examples, and saving/loading weights.
 */
class NeuralNetwork {
public:
    /**
     * @brief Construct a neural network with given layer sizes.
     * @param layerSizes Vector of layer sizes (including input and output).
     * @param activation Activation function for all layers.
     * @param activationDerivative Derivative of activation function.
     */
    NeuralNetwork(const std::vector<int>& layerSizes,
                  const std::vector<std::function<double(double)>>& activations,
                  const std::vector<std::function<double(double)>>& activationDerivatives);

    /**
     * @brief Forward pass through all layers.
     * @param input Input matrix (inputSize x 1)
     * @return Output of last layer
     */
    Matrix forward(const Matrix& input);

    /**
     * @brief Backpropagation through the network.
     * @param expected Expected output (one-hot or target)
     * @param learningRate Learning rate for gradient descent
     */
    void backward(const Matrix& expected, double learningRate);

    /**
     * @brief Train the network for a single input-output pair
     * @param input Input matrix
     * @param expected Expected output matrix
     * @param learningRate Learning rate
     */
    void train(const Matrix& input, const Matrix& expected, double learningRate);

    /**
     * @brief Predict output from input
     * @param input Input matrix
     * @return Output matrix
     */
    Matrix predict(const Matrix& input);

    /**
     * @brief Save network weights to binary file
     * @param filename File path
     */
    void save(const std::string& filename) const;

    /**
     * @brief Load network weights from binary file
     * @param filename File path
     */
    void load(const std::string& filename);

    std::vector<Layer> layers; ///< All layers of the network
};

#endif