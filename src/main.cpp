#include "matrix.hpp"
#include <iostream>

template <typename T> bool getInput(T& var, const std::string& warningMessage = "");

int main()
{
    try
    {
        size_t width = 0;
        getInput(width);
        
        Matrix::Matrix<long double> M(width, width);

        for (size_t i = 0; i < width * width; ++i)
        {
            getInput(M[i / width][i % width]);
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

template <typename T> bool getInput(T& var, const std::string& warningMessage)
{ 
    size_t attempts = 3;
    while (true)
    {
        std::cin >> std::ws >>  var;
        if (std::cin.eof())
        {
            return false; 
        }
        if (std::cin.good()) { break; }

        --attempts;

        if (attempts == 0)
        {
            throw std::runtime_error("Max attempts(3) reached. Exiting...");
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input! Try again";
        if (!warningMessage.empty())
            std::cout << " (" << warningMessage << ")"; std::cout << ":\n";
    }
    return true;
}
