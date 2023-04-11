#include "calc.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <set>
#include <algorithm>
#include <memory>
#include <stdexcept>





static bool is_number(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

static bool is_operator(const std::string& str) {
    std::set<std::string> operators{ "+", "-", "/","atan", "abs"};
    return (operators.find(str) != operators.end());
}

static bool is_binary(const std::string& op) {
    return (op == "+" || op == "-" || op == "/");
    
}

static unsigned priority(const std::string& op) {
    if (op == "/") return 3;
    if (op == "atan" || op == "abs") return 2;
    if (op == "+" || op == "-") return 1;
    else return 0;
}


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

double Exp_Solver::solve() {
    std::vector<std::string> lexems = get_lexems();
    std::unique_ptr<ICalculatable> exp = parsing(lexems);
    double result = exp->Calculate();
    return result;
}

std::vector<std::string> Exp_Solver::get_lexems() {
    //std::cout << expression << std::endl;
    std::vector<std::string> lexems;
    std::stringstream ss(expression);
    std::string lexem;
    while (ss >> lexem) {
        lexems.push_back(lexem);
    }
    return lexems;
}

void Exp_Solver::analyze_operator(std::string op) {
    if (is_binary(op)) {
        std::unique_ptr<ICalculatable> right = std::move(output.top());
        output.pop();

        std::unique_ptr<ICalculatable> left = std::move(output.top());
        output.pop();
        if (op == "+") {
            std::unique_ptr<ICalculatable> result = std::make_unique<Add>(std::move(left), std::move(right));
            output.push(std::move(result));
        }
        else if (op == "-") {
            std::unique_ptr<ICalculatable> result = std::make_unique<Subtraction>(std::move(left), std::move(right));
            output.push(std::move(result));
        }
        else if (op == "/") {
            std::unique_ptr<ICalculatable> result = std::make_unique<Division>(std::move(left), std::move(right));
            output.push(std::move(result));
        }
        
    }
    else {
        //добавить для унарных, когда сделаю их
        std::unique_ptr<ICalculatable> left = std::move(output.top());
        output.pop();
        if (op == "abs") {
            std::unique_ptr<ICalculatable> result = std::make_unique<Absolute>(std::move(left));
            output.push(std::move(result));
        }
        else if (op == "atan") {
            std::unique_ptr<ICalculatable> result = std::make_unique<Arctangens>(std::move(left));
            output.push(std::move(result));
        }

    }
}

//вернет указатель на корень дерева выражений
std::unique_ptr<ICalculatable> Exp_Solver::parsing(const std::vector<std::string>& lexems) {
    std::stack<std::string> operators;
    for (const auto& lexem : lexems) {
        if (is_number(lexem)) {
            //std::cout << "number : " << lexem << std::endl;
            // Если лексема является числом, то создаем объект Number
            double value = std::stod(lexem);
            std::unique_ptr<ICalculatable> number = std::make_unique<Number>(value);
            output.push(std::move(number));
        }
        else if (lexem == "(") {
            // Если лексема является открывающей скобкой, то помещаем ее в стек операторов
            operators.push("(");
        }
        else if (lexem == ")") {
            // Если лексема является закрывающей скобкой, то выполняем операции до тех пор,
            // пока не встретим открывающую скобку
            while (operators.top() != "(" ) {
                std::string op = operators.top();
                operators.pop();
                analyze_operator(op);

            }
            operators.pop(); // Удаляем открывающую скобку из стека операторов
        }

        else if (is_operator(lexem)) {
            // Если токен является оператором , то выполняем операции,
            // пока на вершине стека операторов находится оператор с большим
            // или равным приоритетом
            while (!operators.empty() && priority(operators.top()) >= priority(lexem)) {
                std::string op = operators.top();

                analyze_operator(op);
                operators.pop();
            }

            // Помещаем текущий оператор в стек операторов
            operators.push(lexem);
        }
        else {
            throw std::runtime_error("Invalid lexem");
        }
    }
    // Выполняем оставшиеся операции
    while (!operators.empty()) {
        std::string op = operators.top();

        analyze_operator(op);
        operators.pop();
    }

    // Возвращаем вершину
    return std::move(output.top());
}