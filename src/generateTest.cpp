#include "matrix.hpp"
#include <fstream>
#include <iostream>
#include <random>

const double MIN_DOUBLE_ABOVE_ONE = 1.000000001;
const double MAX_DOUBLE = 100000;
const double MIN_DOUBLE = -100000;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <determinant> " << "<metric>" << " <file_write_to>" << std::endl;
        return 0;
    }

    double det = std::stod(argv[1]);
    size_t metric = std::stoul(argv[2]);

    double diagonal_remainder = det;

    srand(static_cast<unsigned int>(time(nullptr)));
    std::random_device dev;
    std::mt19937 rng(dev());

    Matrix::Matrix<double> matrix(metric, metric);

    for (size_t i = 0; i < metric - 1; ++i)
    {
        std::uniform_real_distribution<double> elem(MIN_DOUBLE_ABOVE_ONE, diagonal_remainder);
        double ii_elem = elem(rng);
        diagonal_remainder /= ii_elem;
        matrix[i][i] = ii_elem;

        for (size_t j = i + 1; j < metric; ++j)
        {
            std::uniform_real_distribution<double> elem(MIN_DOUBLE, MAX_DOUBLE);
            double ij_elem = elem(rng);
            matrix[i][j] = ij_elem;
        }
    }
    matrix[metric - 1][metric - 1] = diagonal_remainder;

    // Путаем строки
    for (size_t i = 1; i < metric; ++i)
    {
        for (size_t c = 0; c < metric; ++c)
            if (i % 2 == 0)
                matrix[i][c] += matrix[i - 1][c];
            else
                matrix[i][c] -= matrix[i - 1][c];
    }

    // Путаем столбцы
    for (size_t c = 1; c < metric; ++c)
    {
        for (size_t r = 0; r < metric; ++r)
        {
            if (c % 2 == 0)
                matrix[r][c] -= matrix[r][c - 1] * 1.1;
            else
                matrix[r][c] += matrix[r][c - 1] / 1.1;
        }
    }

    std::string fileWriteTo;

    if (argc == 4) fileWriteTo = argv[3];

    matrix.dump(fileWriteTo);
    return 0;
}