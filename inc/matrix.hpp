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

namespace Matrix {

template <typename T>
class Matrix {
public:
    // ==============================
    // Вспомогательная структура строки
    // ==============================
    struct ProxyRow
    {
        T* data_ = nullptr;
        size_t size_ = 0;

        T& operator[](size_t col) { return data_[col]; }
        const T& operator[](size_t col) const { return data_[col]; }

        ProxyRow& operator+=(const ProxyRow& other) {
            checkSize(other);
            for (size_t i = 0; i < size_; ++i)
                data_[i] += other.data_[i];
            return *this;
        }

        ProxyRow& operator-=(const ProxyRow& other) {
            checkSize(other);
            for (size_t i = 0; i < size_; ++i)
                data_[i] -= other.data_[i];
            return *this;
        }

        ProxyRow& operator*=(T factor) {
            for (size_t i = 0; i < size_; ++i)
                data_[i] *= factor;
            return *this;
        }

        ProxyRow& operator/=(T factor) {
            for (size_t i = 0; i < size_; ++i)
                data_[i] /= factor;
            return *this;
        }

        // Поиск индекса максимального по модулю элемента в строке
        size_t findABSMax() const {
            if (size_ == 0) return 0;
            size_t idx = 0;
            T maxVal = std::abs(data_[0]);
            for (size_t i = 1; i < size_; ++i) {
                T absVal = std::abs(data_[i]);
                if (absVal > maxVal) {
                    maxVal = absVal;
                    idx = i;
                }
            }
            return idx;
        }

        void checkSize(const ProxyRow& other) const {
            if (size_ != other.size_)
                throw std::invalid_argument("Row size mismatch");
        }
    };

private:
    myVector::myVector<T> m_;             // данные в виде сплошного блока
    myVector::myVector<ProxyRow> rows_;   // указатели на строки
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
        if (data.size() % width != 0)
            throw std::invalid_argument("Vector size does not match matrix dimensions");

        height_ = data.size() / width;
        rows_.reserve(height_);
        for (size_t i = 0; i < height_; ++i)
            rows_.push_back({ &m_[i * width_], width_ });
    }

    Matrix(size_t width, myVector::myVector<T>&& data)
        : m_(std::move(data)), width_(width)
    {
        if (m_.size() % width != 0)
            throw std::invalid_argument("Vector size does not match matrix dimensions");

        height_ = m_.size() / width;
        for (size_t i = 0; i < height_; ++i)
            rows_.push_back({ &m_[i * width_], width_ });
    }

    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;
    Matrix(Matrix&&) = delete;
    Matrix& operator=(Matrix&&) = delete;

    // ==============================
    // Доступ к строкам
    // ==============================
    ProxyRow operator[](size_t i) { return rows_[i]; }
    const ProxyRow operator[](size_t i) const { return rows_[i]; }

    // ==============================
    // Вычисление детерминанта
    // ==============================
    T getDet() const {
        if (height_ == 0 || width_ == 0) throw std::out_of_range("Matrix is empty");
        if (height_ == 1 && width_ == 1) return rows_[0][0];

        myVector::myVector<T> tmp = m_;
        std::vector<ProxyRow> tempRows;
        for (size_t i = 0; i < height_; ++i)
            tempRows.push_back({ &tmp[i * width_], width_ });

        T det = 1;

        for (size_t i = 0; i < height_; ++i)
        {
            // size_t idx_of_absmax = tempRows[i].findABSMax();
            // if (idx_of_absmax != i)
            // {
            //     det = -det;
            //     for (size_t j = i; j < height_; ++j)
            //         std::swap(tempRows[j][i], tempRows[j][idx_of_absmax]);
            // }

            // Если ведущий элемент равен 0 — ищем строку для свапа
            if (tempRows[i][i] == T(0))
            {
                size_t swapRow = i + 1;
                while (swapRow < height_ && tempRows[swapRow][i] == T(0))
                    ++swapRow;
                if (swapRow == height_) return T(0);
                std::swap(tempRows[i], tempRows[swapRow]);
                det = -det;
            }

            det *= tempRows[i][i];

            // Приведение к треугольному виду
            for (size_t j = i + 1; j < height_; ++j) {
                if (tempRows[j][i] == T(0)) continue;
                T factor = tempRows[j][i] / tempRows[i][i];
                for (size_t k = i; k < width_; ++k)
                    tempRows[j][k] -= tempRows[i][k] * factor;
            }
        }

        return det;
    }

    void dump(const std::string& fileWriteTo = "") const
    {
        if (!fileWriteTo.empty())
        {
            std::ofstream ofs(fileWriteTo);
            if (!ofs) throw std::runtime_error("Failed to open file");

            ofs << width_ << std::endl;
            for (size_t i = 0; i < height_; ++i)
            {
                for (size_t j = 0; j < width_; ++j)
                    ofs << std::fixed << std::setprecision(8) << rows_[i][j] << " ";
                
                ofs << std::endl;
            }
        }
        else
        {
            for (size_t i = 0; i < height_; ++i)
            {
                for (size_t j = 0; j < width_; ++j)
                    std::cout << std::fixed << std::setprecision(8) << rows_[i][j] << " ";

                std::cout << std::endl;
            }
        }
    }

    size_t height() const { return height_; }
    size_t width()  const { return width_; }
    size_t size()   const { return height_ * width_; }
};

} // namespace Matrix

#endif // MATRIX_HPP
