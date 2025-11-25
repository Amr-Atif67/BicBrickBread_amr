#include "Layer.h"
#include <iostream>
#include <stdexcept>

Layer::Layer(int inputSize, int neuronCount,
             std::function<double(double)> activation,
             std::function<double(double)> activationDerivative)
    : inputSize(inputSize), neuronCount(neuronCount),
      W(Matrix::random(neuronCount, inputSize, -1.0, 1.0)),
      B(Matrix::random(neuronCount, 1, -1.0, 1.0)),
      Z(neuronCount, 1),
      A(neuronCount, 1),
      lastInput(inputSize, 1),
      activate(activation),
      activateDerivative(activationDerivative)
{ }

Matrix Layer::forward(const Matrix& input){
    lastInput = input;
    Z = (W * input) + B;
    A = Z;
    A.apply(activate);
    return A;
}

Matrix Layer::backward(const Matrix& dC_dA, double learningRate){
    Matrix dC_dZ = dC_dA;
    for(int i=0;i<dC_dZ.rows;i++)
        for(int j=0;j<dC_dZ.cols;j++)
            dC_dZ(i,j) *= activateDerivative(Z(i,j));

    Matrix dC_dW = dC_dZ * lastInput.transpose();
    Matrix dC_dB = dC_dZ;

    for(int i=0;i<W.rows;i++)
        for(int j=0;j<W.cols;j++)
            W(i,j) -= learningRate * dC_dW(i,j);

    for(int i=0;i<B.rows;i++)
        for(int j=0;j<B.cols;j++)
            B(i,j) -= learningRate * dC_dB(i,j);

    Matrix dC_dX = W.transpose() * dC_dZ;
    return dC_dX;
}

const Matrix& Layer::getOutput() const {
    return A;
}

void Layer::save(std::ofstream& out) const {
    out.write(reinterpret_cast<const char*>(W.dataPtr()), W.rows * W.cols * sizeof(double));
    out.write(reinterpret_cast<const char*>(B.dataPtr()), B.rows * B.cols * sizeof(double));
}

void Layer::load(std::ifstream& in) {
    in.read(reinterpret_cast<char*>(W.dataPtr()), W.rows * W.cols * sizeof(double));
    in.read(reinterpret_cast<char*>(B.dataPtr()), B.rows * B.cols * sizeof(double));
}