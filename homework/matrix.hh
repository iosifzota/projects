#ifndef __matrix_hh
#define __matrix_hh

#include <iostream>
#include <istream>
#include <ostream>
#include <vector>
#include "utils.hh"
#include "req.hh"

template<typename T> class matrix;

template<typename T>
std::istream& operator >> (std::istream& input, matrix<T>& mat);

template<typename T>
std::ostream& operator << (std::ostream& output, const matrix<T>& mat);

template<typename T> class matrix {
private:
        T **data;
        unsigned m_rows, m_cols;

        /* Allocate memory. Used by both constructors. */
        static T** allocate_storage(unsigned rows, unsigned cols);

        void allocate(unsigned rows, unsigned cols);
        void realloc(unsigned rows, unsigned cols);

        /* Helper function for () matrix access. */
        inline void check_bounds(unsigned, unsigned) const;

public:

        matrix(unsigned m, unsigned n);
        matrix(unsigned m, unsigned n, T fill_val);
        matrix(const matrix<T>&);
        ~matrix();

        void fill(T val);

        // std::vector<T> find_freq(unsigned max_freq) const;

        /**/
        inline unsigned rows() const;
        inline unsigned cols() const;

        void copy_by_value(const matrix<T>&);

        /* Storage access. */
        inline T& operator()(unsigned row, unsigned col);
        inline const T& operator()(unsigned row, unsigned col) const;

        /* Input handling */
        friend std::istream& operator >> <T>(std::istream&, matrix<T>&);
        friend std::ostream& operator << <T>(std::ostream& output, const matrix<T>& mat);
};


/* ================================================================================
 * #BEGIN: Matrix implementaion
 * ================================================================================
 */
/* Constructor #1 */
template<typename T>
matrix<T>::matrix(unsigned m, unsigned n)
{
        data = nullptr;
        allocate(m, n);
}

/* Constructor #2 */
template<typename T>
matrix<T>::matrix(unsigned m, unsigned n, T fill_val)
{
        data = nullptr;
        allocate(m, n);
        fill(fill_val);
}

template<typename T>
void matrix<T>::realloc(unsigned rows, unsigned cols)
{
        /* If matrix is empty defer to allocate. */
        if (data == nullptr)
                allocate(rows, cols);

        T **new_storage;
        new_storage = allocate_storage(rows, cols);
// HERE

        /* copy values from data to new_storage. */
        /* data <- new_storage */
        /* update m_rows, m_cols. */
}

template<typename T>
void matrix<T>::copy_by_value(const matrix<T>& other)
{
        unsigned min_rows, min_cols;

        min_rows = std::min(m_rows, other.m_rows);
        min_cols = std::min(m_cols, other.m_cols);

        for (unsigned row = 0; row < min_rows; ++row)
                for (unsigned col = 0; col < min_cols; ++col)
                        data[row][col] = other.data[row][col];
}

template<typename T>
void matrix<T>::allocate(unsigned rows, unsigned cols)
{
        req(data == nullptr, "Possible memory leak.");

        data = allocate_storage(rows, cols);
        m_rows = rows;
        m_cols = cols;
}

/* Allocate memory. */
template<typename T>
T** matrix<T>::allocate_storage(unsigned rows, unsigned cols)
{
        req(rows != 0 && cols != 0, "Cannot allocate empty matrix.");

        T **new_storage;

        new_storage = new T*[rows];
        new_storage[0] = new T[rows * cols];

        for (unsigned i = 1; i < rows; ++i)
                new_storage[i] = new_storage[i - 1] + cols;

        return new_storage;
}

/* Fill matrix with a value. */
template<typename T>
void matrix<T>::fill(T val)
{
        for (unsigned row = 0; row < m_rows; ++row)
                for (unsigned col = 0; col < m_cols; ++col)
                        data[row][col] = val;
}

/* Copy constructor. */
template<typename T>
matrix<T>::matrix(const matrix<T>& mat)
{
        m_rows = mat.m_rows;
        m_cols = mat.m_cols;

        data = new T*[m_rows];
        data[0] = new T[m_rows * m_cols];

        for (unsigned i = 1; i < m_rows; ++i)
                data[i] = data[i - 1] + m_cols;

        for (unsigned row = 0; row < m_rows; ++row)
                for (unsigned col = 0; col < m_cols; ++ col)
                        data[row][col] = mat(row, col);
}

// operator >>
template<typename T>
std::istream& operator >> (std::istream& input, matrix<T>& mat)
{
        for (unsigned i = 0; i < mat.m_rows; ++i)
                for (unsigned j = 0; j < mat.m_cols; ++j)
                        req(input >> mat.data[i][j], "Not enough input.");

        return input;
}

// operator <<
template<typename T>
std::ostream& operator << (std::ostream& output, const matrix<T>& mat)
{
        for (unsigned i = 0; i < mat.m_rows; ++i) {
                for (unsigned j = 0; j < mat.m_cols; ++j) {
                        output << mat.data[i][j] << " ";
                }
                output << std::endl;
        }

        return output;
}

/* Matrix access () */
template<typename T>
T& matrix<T>::operator()(unsigned row, unsigned col)
{
        check_bounds(row, col);
        return data[row][col];
}

/* Matrix access () */
template<typename T>
const T& matrix<T>::operator()(unsigned row, unsigned col) const
{
        check_bounds(row, col);
        return data[row][col];
}

/* Helper function for matrix access. */
template<typename T>
void matrix<T>::check_bounds(unsigned row, unsigned col) const
{
        char err_msg[] = "Trying to access matrix out of bounds.";
        req(row < m_rows, err_msg);
        req(col < m_cols, err_msg);
}

template<typename T>
inline unsigned matrix<T>::rows() const
{
        return m_rows;
}

template<typename T>
inline unsigned matrix<T>::cols() const
{
        return m_cols;
}

/* Destructor */
template<typename T>
matrix<T>::~matrix()
{
        delete[] data[0];
        delete[] data;
}
/* ================================================================================
 * #END: Matrix implementaion
 * ================================================================================
 */

#endif


/*
{
        req(rows != 0 && cols != 0, "Cannot allocate empty matrix.");

        T **new_storage;

        new_storage = new T*[rows];
        new_storage[0] = new T[rows * cols];

        for (unsigned i = 1; i < rows; ++i)
                new_storage[i] = new_storage[i - 1] + cols;

        if (data != nullptr) {

                delete[] data[0];
                delete[] data;
        }

        m_rows = rows;
        m_cols = cols;


}

 */
