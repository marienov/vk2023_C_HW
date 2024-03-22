#include "utils.hpp"
#include <string>
#include <stdexcept>
#include <iostream>


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "No expression were passed" << std::endl;
        return 1;
    }
    try {
        std::string str{ argv[1] };
        Exp_Solver solver { str };

        double result = solver.solve();
        std::cout << result << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
