#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <functional>
#include <iostream>
#include <stdexcept>

/**
 * @class Matrix
 * @brief Simple 2D matrix with 1D flattened storage optimized for neural networks.
 *
 * Supports in-place element-wise operations, matrix multiplication, transpose, and
 * convenient construction from vectors or random initialization.
 */
class Matrix {
public:
    int rows;                  ///< Number of rows
    int cols;                  ///< Number of columns
    std::vector<double> data;  ///< Flattened row-major storage

    /** 
     * @brief Construct a matrix.
     * @param rows Number of rows
     * @param cols Number of columns
     * @param zero Initialize elements to zero if true, else 1.0
     */
    Matrix(int rows=0, int cols=0, bool zero=true);

    /**
     * @brief Construct a matrix from a flat vector
     * @param values Flat vector in row-major order
     * @param rows Number of rows
     * @param cols Number of columns
     */
    Matrix(const std::vector<double>& values, int rows, int cols);

    /**
     * @brief Randomly initialize a matrix.
     * @param rows Number of rows
     * @param cols Number of columns
     * @param min Minimum value
     * @param max Maximum value
     * @return Matrix filled with random values
     */
    static Matrix random(int rows, int cols, double min, double max);

    /** Access element (r,c) for writing */
    inline double& operator()(int r, int c) { return data[r*cols + c]; }

    /** Access element (r,c) for reading */
    inline const double& operator()(int r, int c) const { return data[r*cols + c]; }

    /** Pointer to raw data */
    double* dataPtr() { return data.data(); }
    const double* dataPtr() const { return data.data(); }

    /** In-place element-wise function application */
    void apply(const std::function<double(double)>& func);

    /** Element-wise multiplication (Hadamard) */
    void hadamard(const Matrix& other);

    /** Matrix addition */
    Matrix operator+(const Matrix& other) const;

    /** Matrix subtraction */
    Matrix operator-(const Matrix& other) const;

    /** Matrix transpose */
    Matrix transpose() const;

    /** Optimized matrix multiplication: a*b -> result (preallocated) */
    static void multiply(const Matrix& a, const Matrix& b, Matrix& result);

    /** Matrix multiplication convenience operator */
    Matrix operator*(const Matrix& other) const;

    /** Resize matrix (reset elements to 0) */
    void resize(int r, int c);

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

    /** Print matrix to console */
    void print() const;
};

#endif