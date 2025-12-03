#include "Layer.h"
#include <random>
#include <fstream>
#include <stdexcept>

Layer::Layer(int inputSize_, int neuronCount_,
             std::function<double(double)> activation_,
             std::function<double(double)> activationDerivative_)
    : inputSize(inputSize_), neuronCount(neuronCount_),
      W(Matrix::random(neuronCount_, inputSize_, -1.0, 1.0)),
      B(Matrix::random(neuronCount_, 1, -1.0, 1.0)),
      Z(neuronCount_, 1), A(neuronCount_,1), lastInput(inputSize_,1),
      activate(activation_), activateDerivative(activationDerivative_)
{}

/**
 * @brief Forward pass (Z = W*X + B, A = activate(Z))
 */
Matrix Layer::forward(const Matrix& input){
    lastInput = input;
    Matrix::multiply(W, input, Z); // Z = W*X
    for(int i=0;i<neuronCount;i++) Z(i,0) += B(i,0); // add biases in-place
    A = Z;
    A.apply(activate);
    return A;
}

/**
 * @brief Backward pass (gradient descent update)
 */
Matrix Layer::backward(const Matrix& dC_dA, double learningRate){
    // Compute dZ = dC/dA * activate'(Z)
    Matrix dZ = dC_dA;
    for(int i=0;i<neuronCount;i++)
        dZ(i,0) *= activateDerivative(Z(i,0));

    // Update weights: W -= lr * dZ * X^T
    Matrix lastInputT = lastInput.transpose();
    Matrix deltaW(neuronCount, inputSize);
    Matrix::multiply(dZ, lastInputT, deltaW);
    for(int i=0;i<neuronCount;i++)
        for(int j=0;j<inputSize;j++)
            W(i,j) -= learningRate * deltaW(i,j);

    // Update biases: B -= lr * dZ
    for(int i=0;i<neuronCount;i++)
        B(i,0) -= learningRate * dZ(i,0);

    // Return gradient w.r.t input: dC/dX = W^T * dZ
    Matrix Wt = W.transpose();
    Matrix dC_dX(neuronCount,1);
    Matrix::multiply(Wt, dZ, dC_dX);
    return dC_dX;
}

const Matrix& Layer::getOutput() const { return A; }


void Layer::save(std::ofstream& out) const {
    for(int i = 0; i < W.rows; i++){
        for(int j = 0; j < W.cols; j++){
            double val = W(i,j);
            out.write(reinterpret_cast<const char*>(&val), sizeof(double));
        }
    }

    for(int i = 0; i < B.rows; i++){
        double val = B(i,0);
        out.write(reinterpret_cast<const char*>(&val), sizeof(double));
    }
}

void Layer::load(std::ifstream& in){
    if(!in) throw std::runtime_error("Failed to open Layer load stream");

    for(int i = 0; i < W.rows; i++){
        for(int j = 0; j < W.cols; j++){
            double val;
            in.read(reinterpret_cast<char*>(&val), sizeof(double));
            if(!in) throw std::runtime_error("Unexpected EOF while loading weights");
            W(i,j) = val;
        }
    }

    for(int i = 0; i < B.rows; i++){
        double val;
        in.read(reinterpret_cast<char*>(&val), sizeof(double));
        if(!in) throw std::runtime_error("Unexpected EOF while loading biases");
        B(i,0) = val;
    }
}