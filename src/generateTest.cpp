#include "matrix.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <stdexcept>
#include <tuple>

const double MIN_DOUBLE_ABOVE_ONE = 1.000000001;
const double MAX_DOUBLE = 1000;
const double MIN_DOUBLE = -1000;

template <typename T>
std::tuple<T, size_t, std::string> collectArgsData(int argc, char* argv[])
{
    if (argc < 3)
    {
        throw std::invalid_argument("Usage: program <determinant> <metric> [file_write_to]");
    }
    size_t metric = static_cast<size_t>(std::stoul(argv[2]));
    std::string fileWriteTo = (argc == 4) ? argv[3] : "";

    if constexpr (std::is_same_v<T, double>)
        return { std::stod(argv[1]), metric, fileWriteTo };
    else
        return { static_cast<T>(std::stoll(argv[1])), metric, fileWriteTo };
}

template<typename T>
void createMatrixWithDeterminant(Matrix::Matrix<T>& matrix, T det, std::mt19937& rng)
{
    if (matrix.width() != matrix.height())
        throw std::invalid_argument("Matrix must be square");
    size_t metric = matrix.width();

    for(size_t i = 0; i < metric; ++i) {
        for(size_t j = 0; j < metric; ++j) {
            matrix[i][j] = 0;
        }
    }
    if constexpr (std::is_same_v<T, double>)
    {
        double diagonal_remainder = det;
        for (size_t i = 0; i < metric - 1; ++i)
        {
            std::uniform_real_distribution<double> dist_diag(MIN_DOUBLE_ABOVE_ONE, std::abs(diagonal_remainder) + 1.0);
            double ii_elem = dist_diag(rng);
            diagonal_remainder /= ii_elem;
            matrix[i][i] = ii_elem;
        }
        matrix[metric - 1][metric - 1] = diagonal_remainder;
    }
    else
    {
        for (size_t i = 0; i < metric - 1; ++i) {
            matrix[i][i] = 1;
        }
        matrix[metric - 1][metric - 1] = det;
    }
   
    for (size_t i = 0; i < metric; ++i)
    {
        if constexpr (std::is_same_v<T, double>)
        {
            std::uniform_real_distribution<T> dist_upper(MIN_DOUBLE, MAX_DOUBLE);
            for (size_t j = i + 1; j < metric; ++j) matrix[i][j] = dist_upper(rng);
        }
        else
        {
            std::uniform_int_distribution<T> dist_upper(static_cast<T>(MIN_DOUBLE), static_cast<T>(MAX_DOUBLE));
            for (size_t j = i + 1; j < metric; ++j) matrix[i][j] = dist_upper(rng);
        }
    }
}

template<typename T>
void shakeMatrix(Matrix::Matrix<T>& matrix, std::mt19937& rng)
{
    size_t metric = matrix.width();
    if (metric == 0) return;
    for (size_t i = 0; i < metric * 2; ++i)
    {
        std::uniform_int_distribution<size_t> dist(0, metric - 1);
        size_t r1 = dist(rng);
        size_t r2 = dist(rng);

        if (r1 == r2) r2 = (r1 + 1) % metric;
         
        for (size_t c = 0; c < metric; ++c) std::swap(matrix[r1][c], matrix[r2][c]);
    }

    for (size_t i = 0; i < metric * 2; ++i)
    {
        std::uniform_int_distribution<size_t> dist(0, metric - 1);
        size_t c1 = dist(rng);
        size_t c2 = dist(rng);

        if (c1 == c2) c2 = (c1 + 1) % metric;
        
        for (size_t r = 0; r < metric; ++r) std::swap(matrix[r][c1], matrix[r][c2]);
    }
}

template<typename T>
void run(int argc, char* argv[], std::mt19937& rng)
{
    auto [determinant, metric, fileWriteTo] = collectArgsData<T>(argc, argv);

    Matrix::Matrix<T> matrix(metric, metric);
    createMatrixWithDeterminant(matrix, determinant, rng);
    shakeMatrix(matrix, rng);
    matrix.dump(fileWriteTo);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <determinant> <metric> [file_write_to]" << std::endl;
        return 1;
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    try {
        if (std::string(argv[1]).find('.') != std::string::npos)
        {
            run<double>(argc, argv, rng);
        }
        else
        {
            run<int>(argc, argv, rng);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
