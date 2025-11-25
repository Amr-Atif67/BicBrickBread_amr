#ifndef LAYER_H
#define LAYER_H

#include "Matrix.h"
#include <functional>
#include <fstream>

/**
 * @class Layer
 * @brief Fully connected neural network layer.
 *
 * Supports forward and backward propagation, as well as
 * saving and loading weights and biases in binary format.
 */
class Layer {
public:
    /**
     * @brief Construct a fully connected layer.
     * @param inputSize Number of inputs to the layer.
     * @param neuronCount Number of neurons in this layer.
     * @param activation Activation function.
     * @param activationDerivative Derivative of activation function.
     */
    Layer(int inputSize, int neuronCount,
          std::function<double(double)> activation,
          std::function<double(double)> activationDerivative);

    /**
     * @brief Forward pass through the layer.
     * @param input Input matrix (inputSize x 1).
     * @return Output activations (neuronCount x 1).
     */
    Matrix forward(const Matrix& input);

    /**
     * @brief Backward pass for backpropagation.
     * @param dC_dA Gradient of cost w.r.t output activations.
     * @param learningRate Learning rate to update weights.
     * @return Gradient of cost w.r.t input (to propagate backward).
     */
    Matrix backward(const Matrix& dC_dA, double learningRate);

    /**
     * @brief Get last output of this layer.
     * @return Activations of this layer.
     */
    const Matrix& getOutput() const;

    /**
     * @brief Save layer weights and biases to a binary file.
     * @param out Output file stream.
     */
    void save(std::ofstream& out) const;

    /**
     * @brief Load layer weights and biases from a binary file.
     * @param in Input file stream.
     */
    void load(std::ifstream& in);

    int inputSize;   ///< Number of inputs
    int neuronCount; ///< Number of neurons

    Matrix W; ///< Weights (neuronCount x inputSize)
    Matrix B; ///< Biases (neuronCount x 1)
    Matrix Z; ///< Weighted input (W*X + B)
    Matrix A; ///< Activations

    Matrix lastInput; ///< Stores input for backprop

    std::function<double(double)> activate;          ///< Activation function
    std::function<double(double)> activateDerivative; ///< Activation derivative
};

#endif