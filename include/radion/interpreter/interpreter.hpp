#pragma once
#include "radion/interpreter/exceptions.hpp"
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
    void exit(std::string exit_message);
    void exit(RuntimeException* exception);
    std::map<std::string, std::any> symbols;
};

std::string value_to_string(std::any value);
