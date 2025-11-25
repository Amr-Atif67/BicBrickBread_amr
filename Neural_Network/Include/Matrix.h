#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <functional>
#include <iostream>

/**
 * @class Matrix
 * @brief Simple matrix class with flattened storage for neural networks.
 *
 * Stores a 2D matrix in a 1D vector for efficiency.
 */
class Matrix {
public:
    int rows; ///< Number of rows
    int cols; ///< Number of columns
    std::vector<double> data; ///< Flattened 1D data storage

    /**
     * @brief Construct a matrix.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param zero Initialize elements to zero if true, otherwise to one.
     */
    Matrix(int rows, int cols, bool zero = true);

    /**
     * @brief Construct a matrix from a vector of values.
     * @param values Flat vector containing matrix values (row-major).
     * @param rows Number of rows.
     * @param cols Number of columns.
     */
    Matrix(const std::vector<double>& values, int rows, int cols);

    /**
     * @brief Generate a random matrix.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param min Minimum random value.
     * @param max Maximum random value.
     * @return Randomly initialized matrix.
     */
    static Matrix random(int rows, int cols, double min, double max);

    /**
     * @brief Access element at (r, c) for reading/writing.
     * @param r Row index.
     * @param c Column index.
     * @return Reference to element at (r,c).
     */
    double& operator()(int r, int c) { return data[r*cols + c]; }

    /**
     * @brief Access element at (r, c) for read-only.
     * @param r Row index.
     * @param c Column index.
     * @return Const reference to element at (r,c).
     */
    const double& operator()(int r, int c) const { return data[r*cols + c]; }

    /**
     * @brief Apply a function element-wise.
     * @param func Function to apply to each element.
     */
    void apply(const std::function<double(double)>& func);

    /**
     * @brief Element-wise multiplication (Hadamard product) with another matrix.
     * @param other Matrix to multiply element-wise.
     */
    void hadamard(const Matrix& other);

    /**
     * @brief Transpose the matrix.
     * @return Transposed matrix.
     */
    Matrix transpose() const;

    /**
     * @brief Element-wise addition.
     * @param other Matrix to add.
     * @return Resulting matrix.
     */
    Matrix operator+(const Matrix& other) const;

    /**
     * @brief Element-wise subtraction.
     * @param other Matrix to subtract.
     * @return Resulting matrix.
     */
    Matrix operator-(const Matrix& other) const;

    /**
     * @brief Matrix multiplication.
     * @param other Matrix to multiply.
     * @return Resulting matrix.
     * @throws std::runtime_error if dimensions mismatch.
     */
    Matrix operator*(const Matrix& other) const;

    /**
     * @brief Get pointer to raw data.
     * @return Pointer to underlying data array.
     */
    double* dataPtr();

    /**
     * @brief Get const pointer to raw data.
     * @return Const pointer to underlying data array.
     */
    const double* dataPtr() const;

    /**
     * @brief Print matrix to console.
     */
    void print() const;
};

#endif