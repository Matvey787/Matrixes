#include "matrix.hpp"
#include <iostream>


int main()
{
    try
    {
        size_t width = 0;
        std::cin >> width;
        Matrix::Matrix<long double> M(width, width);

        for (size_t i = 0; i < width * width; ++i)
        {
            std::cin >> M[i / width][i % width];
        }
        // M.dump();

        long double det = M.getBareissDet();
        std::cout << det << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
