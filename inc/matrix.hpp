#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "myVector.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <ranges>

namespace Matrix {

namespace detail {

template<typename, typename = void>
struct has_std_abs : std::false_type {};

template<typename T>
struct has_std_abs<T, std::void_t<decltype(std::abs(std::declval<T>()))>> 
    : std::true_type {};

}; // detail

template <typename T>
class Matrix {
public:
    struct ProxyRow
    {
        T* data_ = nullptr;
        size_t size_ = 0;

        T& operator[](size_t col) { return data_[col]; }
        const T& operator[](size_t col) const { return data_[col]; }

        ProxyRow& operator+=(const ProxyRow& other) {
            checkSize(other);

            std::transform(data_, data_ + size_, other.data_, data_, std::plus<T>());
        
            return *this;
        }

        ProxyRow& operator-=(const ProxyRow& other) {
            checkSize(other);
            for (size_t i = 0; i < size_; ++i)
                data_[i] -= other.data_[i];
            return *this;
        }

        ProxyRow& operator*=(const T& factor) {
            for (size_t i = 0; i < size_; ++i)
                data_[i] *= factor;
            return *this;
        }

        ProxyRow& operator/=(const T& factor) {
            for (size_t i = 0; i < size_; ++i)
                data_[i] /= factor;
            return *this;
        }

        size_t findABSMax() const {
            auto abs_data = data_ | std::views::transform([](const T& x) { return std::abs(x); });
            auto It = std::ranges::max_element(abs_data);
            return std::distance(abs_data.begin(), It);
        }

        void checkSize(const ProxyRow& other) const {
            if (size_ != other.size_)
                throw std::invalid_argument("Row size mismatch");
        }
    };

private:
    myVector::myVector<T> m_;             
    myVector::myVector<ProxyRow> rows_;   
    size_t height_ = 0;
    size_t width_  = 0;

public:
    Matrix(size_t height, size_t width)
        : m_(height * width), height_(height), width_(width)
    {
        for (size_t i = 0; i < height; ++i)
            rows_.push_back({ &m_[i * width], width });
    }

    Matrix(size_t width, myVector::myVector<T>& data)
        : m_(data), width_(width)
    {
        if (!detail::has_std_abs<T>::value)
            throw std::invalid_argument("Type T must support std::abs");

        if (data.size() % width != 0)
            throw std::invalid_argument("Vector size does not match matrix dimensions");

        height_ = data.size() / width;
        for (size_t i = 0; i < height_; ++i)
            rows_.push_back({ &m_[i * width_], width_ });
    }

    Matrix(size_t width, myVector::myVector<T>&& data)
        : m_(std::move(data)), width_(width)
    {
        if (!detail::has_std_abs<T>::value)
            throw std::invalid_argument("Type T must support std::abs");

        if (m_.size() % width != 0)
            throw std::invalid_argument("Vector size does not match matrix dimensions");

        height_ = m_.size() / width;
        for (size_t i = 0; i < height_; ++i)
            rows_.push_back({ &m_[i * width_], width_ });
    }

    Matrix(const Matrix& matrix) = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    Matrix& operator=(Matrix&&) = default;

    ProxyRow operator[](size_t i) { return rows_[i]; }
    const ProxyRow operator[](size_t i) const { return rows_[i]; }

    T getGaussDet() const {
        if (height_ == 0 || width_ == 0) throw std::out_of_range("Matrix is empty");
        if (height_ != width_) throw std::invalid_argument("Determinant defined only for square matrices");
        if (height_ == 1 && width_ == 1) return rows_[0][0];

        
        myVector::myVector<T> tmp = m_;
        myVector::myVector<ProxyRow> tempRows;

        for (size_t i = 0; i < height_; ++i)
            tempRows.push_back({ &tmp[i * width_], width_ });

        T det = 1;
        
        for (size_t i = 0; i < height_; ++i)
        {
            
            if (tempRows[i][i] == T(0))
            {
                size_t swapRow = i + 1;
                while (swapRow < height_ && tempRows[swapRow][i] == T(0))
                    ++swapRow;

                if (swapRow == height_)
                {
                    return T(0);
                }
                else
                {
                    std::swap(tempRows[i], tempRows[swapRow]);
                    det = -det;
                    
                    size_t maxColIdx = tempRows[i].findABSMax();
                    if (tempRows[i][maxColIdx] != T(0) && maxColIdx != i)
                    {
                        
                        for (size_t row = 0; row < height_; ++row)
                            std::swap(tempRows[row][i], tempRows[row][maxColIdx]);

                        det = -det; 
                    }
                }
            }

            if (tempRows[i][i] == T(0))
                return T(0); 
            
            det *= tempRows[i][i];

            
            for (size_t j = i + 1; j < height_; ++j)
            {
                if (tempRows[j][i] == T(0)) continue;

                T factor = tempRows[j][i] / tempRows[i][i];
                
                for (size_t k = i; k < width_; ++k)
                    tempRows[j][k] -= tempRows[i][k] * factor;
            }
        }

        return det;
    }

    T getBareissDet() const
    {
        if (height_ == 0 || width_ == 0)
        {
            // std::cout << "here6" << std::endl;
            throw std::out_of_range("Matrix is empty");
        }
            
        if (height_ != width_)
            throw std::invalid_argument("Determinant defined only for square matrices");

        if (height_ == 1) return rows_[0][0];

        // std::cout << "here5" << std::endl;

        myVector::myVector<T> tmp = m_;
        std::vector<ProxyRow> tempRows;
        for (size_t i = 0; i < height_; ++i)
            tempRows.push_back({ &tmp[i * width_], width_ });

        T det_sign = T(1);

        for (size_t k = 0; k < height_ - 1; ++k)
        {
            if (tempRows[k][k] == T(0))
            {
                size_t swapRow = k + 1;
                while (swapRow < height_ && tempRows[swapRow][k] == T(0))
                    ++swapRow;
                if (swapRow == height_)
                    return T(0);
                std::swap(tempRows[k], tempRows[swapRow]);
                det_sign = -det_sign;
            }

            T prevPivot = (k == 0) ? T(1) : tempRows[k-1][k-1];
            for (size_t i = k + 1; i < height_; ++i)
            {
                for (size_t j = k + 1; j < height_; ++j)
                {
                    tempRows[i][j] = (tempRows[k][k] * tempRows[i][j] - tempRows[i][k] * tempRows[k][j]) / prevPivot;
                }
                tempRows[i][k] = 0;
            }
        }

        return det_sign * tempRows[height_ - 1][height_ - 1];
    }

    void dump(const std::string& fileWriteTo = "") const
    {
        std::ofstream ofs(fileWriteTo);

        if (!ofs) throw std::runtime_error("Failed to open dump file");

        dump(ofs);
    }

    void dump(std::ostream& os = std::cout) const
    {
        os << width_ << std::endl;

        for (size_t i = 0; i < height_; ++i)
        {
            for (size_t j = 0; j < width_; ++j)
            {
                os << std::fixed << std::setprecision(8) << rows_[i][j] << " ";
            }
            os << std::endl;
        }
    }


    size_t height() const { return height_; }
    size_t width()  const { return width_; }
    size_t size()   const { return height_ * width_; }
};

} 

#endif 
