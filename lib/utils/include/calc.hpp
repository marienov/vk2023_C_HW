#pragma once
#include <memory>
#include <stdexcept>
#include <cmath>

class ICalculatable {
public:
    virtual double Calculate() const = 0;
};


class Number : public ICalculatable {
public:
    Number(double value_) : value(value_) {}

    double Calculate() const override {
        return value;
    }

private:
    double value;
};

//Бинарные
class Add : public ICalculatable {
public:
    Add( std::unique_ptr<ICalculatable> left_, std::unique_ptr<ICalculatable> right_)
        : left(std::move(left_)), right(std::move(right_)) {}
    double Calculate() const override {
        return left->Calculate() + right->Calculate();
    }

private:
    std::unique_ptr<ICalculatable> left;
    std::unique_ptr<ICalculatable> right;
};

class Subtraction : public ICalculatable {
public:
    Subtraction(std::unique_ptr<ICalculatable> left_, std::unique_ptr<ICalculatable> right_)
        : left(std::move(left_)), right(std::move(right_)) {}
    double Calculate() const override {
        return left->Calculate() - right->Calculate();
    }
private:
    std::unique_ptr<ICalculatable> left;
    std::unique_ptr<ICalculatable> right;
};

class Division : public ICalculatable {
public:
    Division(std::unique_ptr<ICalculatable> left_, std::unique_ptr<ICalculatable> right_)
        : left(std::move(left_)), right(std::move(right_)) {}
    double Calculate() const override {
        double denominator = right->Calculate();
        if (denominator == 0) throw std::runtime_error("Division by zero");
        return left->Calculate() / denominator ;
    }
private:
    std::unique_ptr<ICalculatable> left;
    std::unique_ptr<ICalculatable> right;
};


//Унарные
class Arctangens : public ICalculatable {
public:
    Arctangens(std::unique_ptr<ICalculatable> left_)
        : left(std::move(left_)) {}
    double Calculate() const override {
        return atan(left->Calculate());
    }
private:
    std::unique_ptr<ICalculatable> left;
};


class Absolute : public ICalculatable {
public:
    Absolute(std::unique_ptr<ICalculatable> left_)
        : left(std::move(left_)) {}
    double Calculate() const override {
        return abs(left->Calculate());
    }

private:
    std::unique_ptr<ICalculatable> left;
};