#include "Matrix.h"
#include <random>
#include <cmath>
#include <iomanip>

/*--------------------------------------------------------------
 | Constructors
 *-------------------------------------------------------------*/

template <class T>
Matrix<T>::Matrix(int rows_, int cols_, bool zero)
    : rows(rows_), cols(cols_) 
{
    data.assign(rows * cols, zero ? T(0) : T(1));
}

template <class T>
Matrix<T>::Matrix(const std::vector<T>& values, int rows_, int cols_)
    : rows(rows_), cols(cols_), data(values)
{
    if (values.size() != size_t(rows * cols))
        throw std::runtime_error("Matrix constructor: size mismatch");
}

/*--------------------------------------------------------------
 | Random initializer
 *-------------------------------------------------------------*/

template <class T>
Matrix<T> Matrix<T>::random(int rows, int cols, T min, T max) {
    Matrix<T> m(rows, cols);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<T> dist(min, max);

    for (auto &v : m.data) 
        v = dist(rng);

    return std::move(m);
}

/*--------------------------------------------------------------
 | Element-wise operations
 *-------------------------------------------------------------*/

template <class T>
void Matrix<T>::apply(const std::function<T(T)>& func) {
    for (auto &v : data) 
        v = func(v);
}

template <class T>
void Matrix<T>::hadamard(const Matrix<T>& other) {
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Hadamard: size mismatch");

    for (int i = 0; i < rows * cols; i++)
        data[i] *= other.data[i];
}

/*--------------------------------------------------------------
 | Matrix addition
 *-------------------------------------------------------------*/

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Addition: size mismatch");

    Matrix<T> res(rows, cols);

    for (int i = 0; i < rows * cols; i++)
        res.data[i] = data[i] + other.data[i];

    return std::move(res);
}

/*--------------------------------------------------------------
 | Matrix subtraction
 *-------------------------------------------------------------*/

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Subtraction: size mismatch");

    Matrix<T> res(rows, cols);

    for (int i = 0; i < rows * cols; i++)
        res.data[i] = data[i] - other.data[i];

    return std::move(res);
}

/*--------------------------------------------------------------
 | Transpose
 *-------------------------------------------------------------*/

template <class T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> res(cols, rows);

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            res(c, r) = (*this)(r, c);

    return std::move(res);
}

/*--------------------------------------------------------------
 | Matrix multiplication
 *-------------------------------------------------------------*/

template <class T>
void Matrix<T>::multiply(const Matrix<T>& a, const Matrix<T>& b, Matrix<T>& result) {
    if (a.cols != b.rows)
        throw std::runtime_error("Multiply: size mismatch");

    result.resize(a.rows, b.cols);

    for (int r = 0; r < a.rows; r++) {
        for (int c = 0; c < b.cols; c++) {

            T sum = T(0);

            for (int k = 0; k < a.cols; k++)
                sum += a.data[r * a.cols + k] * b.data[k * b.cols + c];

            result.data[r * result.cols + c] = sum;
        }
    }
}

template <class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    Matrix<T> res(rows, other.cols);
    multiply(*this, other, res);
    return std::move(res);
}

/*--------------------------------------------------------------
 | Scalar multiplication
 *-------------------------------------------------------------*/

template <class T>
Matrix<T> Matrix<T>::operator*(const int val) const {
    Matrix<T> res(rows, cols);
    for (int i = 0; i < rows * cols; i++)
        res.data[i] = data[i] * val;
    return std::move(res);
}

/*--------------------------------------------------------------
 | Resize
 *-------------------------------------------------------------*/

template <class T>
void Matrix<T>::resize(int r, int c) {
    if (rows != r || cols != c) {
        rows = r;
        cols = c;
        data.assign(r * c, T(0));
    }
}

/*--------------------------------------------------------------
 | Output formatting
 *-------------------------------------------------------------*/

template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
    os << std::fixed << std::setprecision(4);

    for (int r = 0; r < m.rows; r++) {
        os << "[ ";
        for (int c = 0; c < m.cols; c++) {
            os << std::setw(10) << m(r, c);
            if (c < m.cols - 1) os << ", ";
        }
        os << " ]\n";
    }

    return os;
}

/*--------------------------------------------------------------
 | Explicit template instantiation for double
 *-------------------------------------------------------------*/

template class Matrix<double>;
template std::ostream& operator<<(std::ostream&, const Matrix<double>&);