#pragma once
#include <vector>
#include <string>
#include <stack>
#include <memory>
#include "calc.hpp"

class Exp_Solver {
public:
    Exp_Solver(const std::string& exp): expression(exp) {}
    double solve();
private:
    std::vector<std::string> get_lexems();
    void analyze_operator(std::string op);
    std::unique_ptr<ICalculatable> parsing(const std::vector<std::string>& lexems);

    std::stack<std::unique_ptr<ICalculatable>> output;
    const std::string& expression;
};