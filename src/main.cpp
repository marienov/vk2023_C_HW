#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include "utils.hpp"


int main(int argc, char* argv[]) {
    std::ifstream fbas;
    std::ifstream faka;
    std::ifstream frat;

    std::set<Series> chosen;
    int minutes = input_params(argc, argv, fbas, frat, faka);
    if (minutes < 0 ){
        return 1;
    }
    choose_series(fbas, frat, faka, minutes, chosen);
    print_set(chosen);
    
    frat.close();
    fbas.close();
    faka.close();
    return 0;
}

