#pragma once
#include "radion/interpreter/exceptions.hpp"
#include "radion/interpreter/symboltable.hpp"
#include "radion/parser/nodes/arithmetic.hpp"
#include "radion/parser/node.hpp"
#include <map>
#include <string>
#include <any>

class Interpreter {
public:
    Interpreter();
    std::any evaluate(Node* program);

    std::any evaluate_arithemtic(ArithmeticNode* expression);
    bool evaluate_boolean(Node* expression);
    int to_int(std::any v);

    void table_descend();
    void table_ascend();

    template <typename T>
    T expect_any(std::any v);
    
    bool nofree = false;
    SymbolTable* symbols;
private:
    void exit(std::string exit_message);
    void exit(RuntimeException* exception);

    std::any returned;
};

std::string value_to_string(std::any value);
bool any_equals(std::any a, std::any b);
bool cmp_any_int(std::any a, std::any b, std::function<bool(int, int)> comparison);

std::string get_typename(const std::type_info &t);

void printAST(Node* root);
