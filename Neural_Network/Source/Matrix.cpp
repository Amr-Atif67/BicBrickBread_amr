#include "Matrix.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <cmath>

/// Constructor: create a matrix with optional zero initialization
Matrix::Matrix(int rows, int cols, bool zero)
    : rows(rows), cols(cols), data(rows * cols, zero ? 0.0 : 1.0) { }

/// Constructor: create a matrix from a flat vector of values
Matrix::Matrix(const std::vector<double>& values, int rows, int cols)
    : rows(rows), cols(cols), data(values) {
    if (values.size() != rows * cols)
        throw std::runtime_error("Vector size does not match matrix dimensions");
}

/// Generate a random matrix in [min, max]
Matrix Matrix::random(int rows, int cols, double min, double max) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Matrix m(rows, cols);
    for (int i = 0; i < rows * cols; ++i)
        m.data[i] = min + ((double)std::rand() / RAND_MAX) * (max - min);
    return m;
}

/// Apply a function element-wise
void Matrix::apply(const std::function<double(double)>& func) {
    for (int i = 0; i < rows * cols; ++i)
        data[i] = func(data[i]);
}

/// Element-wise multiplication (Hadamard product)
void Matrix::hadamard(const Matrix& other) {
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Hadamard: dimension mismatch");
    for (int i = 0; i < rows * cols; ++i)
        data[i] *= other.data[i];
}

/// Transpose the matrix
Matrix Matrix::transpose() const {
    Matrix t(cols, rows);
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            t(c, r) = (*this)(r, c);
    return t;
}

/// Element-wise addition
Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Addition: dimension mismatch");
    Matrix res(rows, cols);
    for (int i = 0; i < rows * cols; ++i)
        res.data[i] = data[i] + other.data[i];
    return res;
}

/// Element-wise subtraction
Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Subtraction: dimension mismatch");
    Matrix res(rows, cols);
    for (int i = 0; i < rows * cols; ++i)
        res.data[i] = data[i] - other.data[i];
    return res;
}

/// Matrix multiplication
Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows)
        throw std::runtime_error("Multiplication: size mismatch");
    Matrix res(rows, other.cols, true);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < other.cols; ++c) {
            double sum = 0.0;
            for (int k = 0; k < cols; ++k)
                sum += (*this)(r, k) * other(k, c);
            res(r, c) = sum;
        }
    }
    return res;
}

/// Pointer to raw data
double* Matrix::dataPtr() {
    return data.data();
}

/// Const pointer to raw data
const double* Matrix::dataPtr() const {
    return data.data();
}

/// Print matrix to console
void Matrix::print() const {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c)
            std::cout << (*this)(r, c) << " ";
        std::cout << "\n";
    }
}