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
 * @brief Fully connected feedforward neural network using updated Layer class.
 *
 * This class manages a sequence of Layers, each containing its own weights,
 * biases, and activation functions. It provides a simple interface for
 * forward propagation, backpropagation, training on individual samples,
 * and persisting/loading network weights to/from files.
 *
 * Responsibilities (SRP):
 *  - Maintain the ordered list of layers and their internal states
 *  - Perform forward pass through all layers
 *  - Perform backward pass and weight updates (gradient descent)
 *  - Provide convenience functions for training and prediction
 *  - Save/load layer parameters (weights and biases) to/from binary files
 */
class NeuralNetwork {
public:
    /**
     * @brief Construct a neural network with specified layer sizes and activations.
     *
     * @param layerSizes Vector specifying the number of neurons in each layer (including input and output).
     * @param activations Vector of activation functions for each layer (excluding input layer).
     * @param activationDerivatives Vector of derivatives of the activation functions for backpropagation.
     */
    NeuralNetwork(const std::vector<int>& layerSizes,
                  const std::vector<std::function<double(double)>>& activations,
                  const std::vector<std::function<double(double)>>& activationDerivatives);

    /**
     * @brief Perform a forward pass through all layers.
     * @param input Input vector (Matrix<double> with inputSize x 1 dimensions)
     * @return Output of the final layer (activations)
     */
    Matrix<double> forward(const Matrix<double>& input);

    /**
     * @brief Perform backward propagation through all layers and update weights.
     * @param expected Expected output (target vector)
     * @param learningRate Learning rate for weight updates
     */
    void backward(const Matrix<double>& expected, double learningRate);

    /**
     * @brief Train the network on a single input-output pair.
     * @param input Input vector
     * @param expected Expected output vector
     * @param learningRate Learning rate
     */
    void train(const Matrix<double>& input, const Matrix<double>& expected, double learningRate);

    /**
     * @brief Predict output from a given input without performing weight updates.
     * @param input Input vector
     * @return Predicted output vector
     */
    Matrix<double> predict(const Matrix<double>& input);

    /**
     * @brief Save all layer weights and biases to a binary file.
     * @param filename Path to file
     */
    void save(const std::string& filename) const;

    /**
     * @brief Load all layer weights and biases from a binary file.
     * @param filename Path to file
     */
    void load(const std::string& filename);

private:
    std::vector<Layer> layers_; ///< Private container for layers (encapsulation)
};

#endif // NEURALNETWORK_H