#ifndef LAYER_H
#define LAYER_H

#include "Matrix.h"
#include <functional>
#include <fstream>

/**
 * @class Layer
 * @brief Fully connected neural network layer with flattened weights.
 *
 * Supports forward/backward propagation and saving/loading weights and biases.
 * Designed for high-performance use with the Matrix class.
 */
class Layer {
public:
    int inputSize;          ///< Number of inputs
    int neuronCount;        ///< Number of neurons

    Matrix W;               ///< Weights (neuronCount x inputSize)
    Matrix B;               ///< Biases (neuronCount x 1)
    Matrix Z;               ///< Weighted input (W*X + B)
    Matrix A;               ///< Activations
    Matrix lastInput;       ///< Stores input for backprop

    std::function<double(double)> activate;          ///< Activation function
    std::function<double(double)> activateDerivative; ///< Activation derivative

    /**
     * @brief Construct a fully connected layer.
     * @param inputSize Number of inputs
     * @param neuronCount Number of neurons
     * @param activation Activation function
     * @param activationDerivative Derivative of activation
     */
    Layer(int inputSize, int neuronCount,
          std::function<double(double)> activation,
          std::function<double(double)> activationDerivative);

    /**
     * @brief Forward pass through the layer.
     * @param input Input vector (inputSize x 1)
     * @return Output activations (neuronCount x 1)
     */
    Matrix forward(const Matrix& input);

    /**
     * @brief Backward pass for backpropagation.
     * Updates weights and biases.
     * @param dC_dA Gradient of cost w.r.t output activations
     * @param learningRate Learning rate
     * @return Gradient w.r.t input to propagate backward
     */
    Matrix backward(const Matrix& dC_dA, double learningRate);

    /**
     * @brief Get last output of this layer
     * @return Activations matrix
     */
    const Matrix& getOutput() const;

    /**
     * @brief Save weights and biases to binary file
     * @param out Output file stream
     */
    void save(std::ofstream& out) const;

    /**
     * @brief Load weights and biases from binary file
     * @param in Input file stream
     */
    void load(std::ifstream& in);
};

#endif