#ifndef LAYER_H
#define LAYER_H

#include <functional>
#include "Matrix.h"

/**
 * @brief Fully-connected neural network layer (refactored for SOLID & encapsulation).
 *
 * RESPONSIBILITIES (SRP):
 *  - Own only the data & math needed for a single dense layer.
 *  - Perform forward/backward propagation using injected activation functions.
 *
 * NOT responsible for:
 *  - Training loop logic
 *  - Loss computation
 *  - Network-wide serialization
 *  - Optimizer logic
 *
 * This design preserves encapsulation, removes unnecessary dependencies,
 * and follows DIP by accepting activation behavior as function objects.
 */
class Layer {
private:
    int inputSize_;        ///< Number of inputs to the layer
    int neuronCount_;      ///< Number of output neurons

    Matrix<double> W_;     ///< Weight matrix (neurons × inputs)
    Matrix<double> B_;     ///< Bias vector (neurons × 1)
    Matrix<double> Z_;     ///< Weighted sums (W·X + B)
    Matrix<double> A_;     ///< Activations
    Matrix<double> lastInput_; ///< Cached input for backprop

    /**
     * @brief Activation function: f(x)
     * Stored as std::function to avoid interface inheritance.
     */
    std::function<double(double)> activationFunc_;

    /**
     * @brief Activation derivative: f'(x)
     * Applied to Z during backprop.
     */
    std::function<double(double)> activationDeriv_;

public:
    /**
     * @brief Construct a dense layer.
     * @param inputSize  Number of inputs
     * @param neurons    Number of neurons
     * @param act        Activation function
     * @param actDeriv   Activation derivative function
     */
    Layer(int inputSize,
          int neurons,
          std::function<double(double)> act,
          std::function<double(double)> actDeriv);

    /**
     * @brief Forward propagation.
     * @param input  Input matrix (inputSize × batchSize)
     * @return Activation matrix reference
     */
    const Matrix<double>& forward(const Matrix<double>& input);

    /**
     * @brief Backward propagation.
     * @param dC_dA  Gradient from next layer
     * @param lr     Learning rate
     * @return dC/dX (gradient to send to previous layer)
     */
    Matrix<double> backward(const Matrix<double>& dC_dA, double lr);

    /** @return Layer output after forward() */
    const Matrix<double>& output() const { return A_; }

    /** @brief Save layer parameters */
    void save(std::ofstream& out) const;

    /** @brief Load layer parameters */
    void load(std::ifstream& in);

    int inputs() const { return inputSize_; }
    int neurons() const { return neuronCount_; }
};

#endif // LAYER_H