#include "matrix.hpp"
#include <iostream>

int main()
{
    try
    {
        size_t width = 0;
        std::cin >> width;
        std::cout << "here1" << std::endl;
        Matrix::Matrix<long double> M(width, width);
        std::cout << "here2" << std::endl;

        for (size_t i = 0; i < width * width; ++i)
        {
            std::cin >> M[i / width][i % width];
        }
        std::cout << "here3 " << M.height() << " " << M.width() << std::endl;
        // M.dump();

        long double det = M.getBareissDet();
        std::cout << "here4" << std::endl;
        std::cout << det << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
