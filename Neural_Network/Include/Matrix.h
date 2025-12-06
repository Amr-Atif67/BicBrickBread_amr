#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <functional>
#include <iostream>
#include <stdexcept>

/**
 * @class Matrix
 * @brief Lightweight 2D matrix with contiguous row-major storage.
 *
 * Designed primarily for neural-network operations:
 *  - Efficient indexing via a flattened vector
 *  - Minimal dynamic allocations
 *  - Element-wise ops, Hadamard ops, transpose, matrix multiply
 *
 * The class DOES NOT perform implicit shape broadcasting.
 * All operations validate dimensional consistency.
 *
 * @tparam T  Numeric type (float, double, int, etc.)
 */
template <class T>
class Matrix {
public:
    int rows;                ///< Number of rows
    int cols;                ///< Number of columns
    std::vector<T> data;     ///< Row-major flattened storage of size rows*cols

    // ---------------------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------------------

    /**
     * @brief Construct a matrix of size (rows x cols).
     * @param rows Number of rows
     * @param cols Number of columns
     * @param zero If true, initialize all elements to 0.
     *             If false, initialize elements to 1.
     *
     * @note Elements are NOT left uninitialized. This simplifies usage in NN code.
     */
    Matrix(int rows = 0, int cols = 0, bool zero = true);

    /**
     * @brief Construct a matrix from a flat row-major vector.
     * @param values Vector of size rows*cols
     * @param rows Number of rows
     * @param cols Number of columns
     *
     * @throws std::runtime_error if values.size() != rows*cols
     */
    Matrix(const std::vector<T>& values, int rows, int cols);

    // ---------------------------------------------------------------------
    // Factories
    // ---------------------------------------------------------------------

    /**
     * @brief Create a matrix filled with uniform random values in [min, max].
     * @param rows Number of rows
     * @param cols Number of columns
     * @param min Lower bound
     * @param max Upper bound
     */
    static Matrix random(int rows, int cols, T min, T max);

    // ---------------------------------------------------------------------
    // Element Access
    // ---------------------------------------------------------------------

    /**
     * @brief Access element (r, c) by reference.
     * @warning No bounds check in release mode.
     */
    inline T& operator()(int r, int c) { return data[r * cols + c]; }

    /**
     * @brief Read-only access to element (r, c).
     */
    inline const T& operator()(int r, int c) const { return data[r * cols + c]; }

    /**
     * @brief Raw pointer to underlying storage.
     * Useful for interoperability with BLAS and LibTorch custom ops.
     */
    T* dataPtr() { return data.data(); }
    const T* dataPtr() const { return data.data(); }

    // ---------------------------------------------------------------------
    // Element-wise operations
    // ---------------------------------------------------------------------

    /**
     * @brief Apply a function f(x) to each element (in place).
     * @param func Mapping function
     *
     * Example: m.apply([](float x){ return std::tanh(x); });
     */
    void apply(const std::function<T(T)>& func);

    /**
     * @brief In-place Hadamard (element-wise) product: this = this ⊙ other.
     * @throws std::runtime_error if shapes differ
     */
    void hadamard(const Matrix& other);

    // ---------------------------------------------------------------------
    // Arithmetic operators
    // ---------------------------------------------------------------------

    /**
     * @brief Matrix addition. Dimensions must match.
     */
    Matrix operator+(const Matrix& other) const;

    /**
     * @brief Matrix subtraction. Dimensions must match.
     */
    Matrix operator-(const Matrix& other) const;

    /**
     * @brief Right-hand matrix multiplication: C = A * B.
     * @throws std::runtime_error if A.cols != B.rows
     */
    Matrix operator*(const Matrix& other) const;

    /**
     * @brief Scalar multiplication: each element multiplied by val.
     */
    Matrix operator*(int val) const;

    // ---------------------------------------------------------------------
    // Structural operations
    // ---------------------------------------------------------------------

    /**
     * @brief Returns the transpose of the matrix.
     */
    Matrix transpose() const;

    /**
     * @brief Resize matrix to (r x c) and reset all elements to zero.
     */
    void resize(int r, int c);

    /**
     * @brief Optimized matrix multiplication into a preallocated matrix.
     *
     * @param a Left operand
     * @param b Right operand
     * @param result Preallocated matrix with dimensions (a.rows x b.cols)
     *
     * @note Does not allocate memory. Caller ensures correct size.
     */
    static void multiply(const Matrix& a, const Matrix& b, Matrix& result);

    // ---------------------------------------------------------------------
    // I/O helpers
    // ---------------------------------------------------------------------

    /**
     * @brief Stream output operator for debugging.
     */
    template <typename J>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<J>& m);
};

#endif