#pragma once
#include "radion/parser/nodes/arithmetic.hpp"
#include "radion/parser/node.hpp"
#include <map>
#include <string>
#include <any>

class Interpreter {
public:
    Interpreter();
    std::any evaluate(Node* program);

    int evaluate_arithemtic(ArithmeticNode* expression);
    void evaluate_boolean(Node* expression);
private:
    std::map<std::string, std::any> symbols;
};
