#include "matrix.hpp"
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
using bigInt = boost::multiprecision::cpp_int;

int main()
{
    try
    {
        size_t width = 0;
        std::cin >> width;
        Matrix::Matrix<bigInt> M(width, width);

        for (size_t i = 0; i < width * width; ++i)
        {
            std::cin >> M[i / width][i % width];
        }
        // M.dump();

        bigInt det = M.getBareissDet();
        std::cout << det << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
