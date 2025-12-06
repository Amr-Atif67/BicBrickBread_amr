#include "../Include/Layer.h"
#include <stdexcept>
#include <fstream>

Layer::Layer(int inputSize,
             int neuronCount,
             std::function<double(double)> act,
             std::function<double(double)> actDeriv)
    : inputSize_(inputSize),
      neuronCount_(neuronCount),
      W_(Matrix<double>::random(neuronCount_, inputSize_, -1.0, 1.0)),
      B_(Matrix<double>::random(neuronCount_, 1, -1.0, 1.0)),
      Z_(neuronCount_, 1),
      A_(neuronCount_, 1),
      lastInput_(inputSize_, 1),
      activationFunc_(act),
      activationDeriv_(actDeriv)
{}

const Matrix<double>& Layer::forward(const Matrix<double>& input) {
    lastInput_ = input;

    Matrix<double>::multiply(W_, input, Z_);

    Z_ = Z_ + B_;

    A_ = Z_;
    A_.apply(activationFunc_);

    return A_;
}

Matrix<double> Layer::backward(const Matrix<double>& dC_dA, double lr) {
    Matrix<double> dZ = dC_dA;
    for (int i = 0; i < neuronCount_; i++)
        dZ(i, 0) *= activationDeriv_(Z_(i, 0));

    Matrix<double> X_T = lastInput_.transpose();
    Matrix<double> deltaW(neuronCount_, inputSize_);
    Matrix<double>::multiply(dZ, X_T, deltaW);

    W_ = W_ - deltaW * lr;

    B_ = B_ - dZ * lr;

    Matrix<double> W_T = W_.transpose();
    Matrix<double> dC_dX(inputSize_, 1);
    Matrix<double>::multiply(W_T, dZ, dC_dX);

    return dC_dX;
}

void Layer::save(std::ofstream& out) const {
    for (int i = 0; i < W_.rows; i++)
        for (int j = 0; j < W_.cols; j++) {
            double v = W_(i, j);
            out.write((char*)&v, sizeof(double));
        }

    for (int i = 0; i < B_.rows; i++) {
        double v = B_(i, 0);
        out.write((char*)&v, sizeof(double));
    }
}

void Layer::load(std::ifstream& in) {
    if (!in) throw std::runtime_error("Layer::load - bad input stream");

    for (int i = 0; i < W_.rows; i++) {
        for (int j = 0; j < W_.cols; j++) {
            double v;
            in.read((char*)&v, sizeof(double));
            if (!in) throw std::runtime_error("Unexpected EOF while loading W");
            W_(i, j) = v;
        }
    }   

    for (int i = 0; i < B_.rows; i++) {
        double v;
        in.read((char*)&v, sizeof(double));
        if (!in) throw std::runtime_error("Unexpected EOF while loading B");
        B_(i, 0) = v;
    }
}