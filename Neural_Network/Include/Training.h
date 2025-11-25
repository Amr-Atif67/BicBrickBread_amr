#ifndef TRAINING_H
#define TRAINING_H

#include "../Neural_Network/Include/NeuralNetwork.h"
#include <vector>
#include <functional>

/**
 * @class Training
 * @brief Provides training utilities for a NeuralNetwork.
 *
 * Supports batch or stochastic gradient descent training.
 */
class Training {
public:
    /**
     * @brief Construct a trainer for a given network.
     * @param network Reference to neural network to train.
     */
    Training(NeuralNetwork& network);

    /**
     * @brief Train network on a dataset for one epoch.
     * @param inputs Vector of input matrices (each inputSize x 1).
     * @param targets Vector of expected output matrices (each outputSize x 1).
     * @param learningRate Learning rate for gradient descent.
     */
    void trainEpoch(const std::vector<Matrix>& inputs,
                    const std::vector<Matrix>& targets,
                    double learningRate);

    /**
     * @brief Train network for multiple epochs.
     * @param inputs Vector of input matrices.
     * @param targets Vector of expected output matrices.
     * @param learningRate Learning rate.
     * @param epochs Number of training epochs.
     * @param verbose Print progress if true.
     */
    void train(const std::vector<Matrix>& inputs,
               const std::vector<Matrix>& targets,
               double learningRate,
               int epochs,
               bool verbose = true);

private:
    NeuralNetwork& network; ///< Reference to network being trained

    /**
     * @brief Compute mean squared error between predicted and target outputs.
     * @param predicted Output from network
     * @param target Expected output
     * @return MSE
     */
    double meanSquaredError(const Matrix& predicted, const Matrix& target) const;
};

#endif