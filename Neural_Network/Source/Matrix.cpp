#include "Matrix.h"
#include <random>
#include <cmath>
#include <iomanip>

Matrix::Matrix(int rows_, int cols_, bool zero) : rows(rows_), cols(cols_) {
    data.assign(rows*cols, zero ? 0.0 : 1.0);
}

Matrix::Matrix(const std::vector<double>& values, int rows_, int cols_) 
    : rows(rows_), cols(cols_), data(values) {
    if(values.size() != size_t(rows*cols))
        throw std::runtime_error("Matrix constructor: size mismatch");
}

Matrix Matrix::random(int rows, int cols, double min, double max){
    Matrix m(rows, cols);
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(min, max);
    for(auto &val : m.data) val = dist(rng);
    return m;
}

void Matrix::apply(const std::function<double(double)>& func){
    for(auto &val : data) val = func(val);
}

void Matrix::hadamard(const Matrix& other){
    if(rows != other.rows || cols != other.cols)
        throw std::runtime_error("Hadamard: size mismatch");
    for(int i=0;i<rows*cols;i++) data[i] *= other.data[i];
}

Matrix Matrix::operator+(const Matrix& other) const {
    if(rows != other.rows || cols != other.cols)
        throw std::runtime_error("Addition: size mismatch");
    Matrix res(rows, cols);
    for(int i=0;i<rows*cols;i++) res.data[i] = data[i] + other.data[i];
    return res;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if(rows != other.rows || cols != other.cols)
        throw std::runtime_error("Subtraction: size mismatch");
    Matrix res(rows, cols);
    for(int i=0;i<rows*cols;i++) res.data[i] = data[i] - other.data[i];
    return res;
}

Matrix Matrix::transpose() const {
    Matrix res(cols, rows);
    for(int r=0;r<rows;r++)
        for(int c=0;c<cols;c++)
            res(c,r) = (*this)(r,c);
    return res;
}

void Matrix::multiply(const Matrix& a, const Matrix& b, Matrix& result){
    if(a.cols != b.rows) throw std::runtime_error("Multiplication: size mismatch");
    result.resize(a.rows, b.cols);

    for(int r=0;r<a.rows;r++){
        for(int c=0;c<b.cols;c++){
            double sum = 0.0;
            for(int k=0;k<a.cols;k++)
                sum += a.data[r*a.cols + k] * b.data[k*b.cols + c];
            result.data[r*result.cols + c] = sum;
        }
    }
}

Matrix Matrix::operator*(const Matrix& other) const {
    Matrix res(rows, other.cols);
    multiply(*this, other, res);
    return res;
}

void Matrix::resize(int r, int c){
    if(rows != r || cols != c){
        rows = r;
        cols = c;
        data.assign(r*c, 0.0);
    }
}

std::ostream& operator<<(std::ostream &os, const Matrix &m)
{
    os << std::fixed << std::setprecision(4);

    for (int r = 0; r < m.rows; ++r) {
        os << "[ ";
        for (int c = 0; c < m.cols; ++c) {
            os << std::setw(10) << m(r, c);
            if (c < m.cols - 1) os << ", ";
        }
        os << " ]\n";
    }

    return os;
}

void Matrix::print() const {
    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++)
            std::cout << (*this)(r,c) << " ";
        std::cout << "\n";
    }
}