#pragma once
#include "radion/interpreter/exceptions.hpp"
#include "radion/interpreter/symboltable.hpp"
#include "radion/interpreter/value.hpp"
#include "radion/parser/nodes/arithmetic.hpp"
#include "radion/parser/node.hpp"
#include <map>
#include <string>
#include <functional>

class Interpreter {
public:
    Interpreter();
    Value* evaluate(Node* program);

    Value* evaluate_arithemtic(ArithmeticNode* expression);
    bool evaluate_boolean(Node* expression);

    void table_descend();
    void table_ascend();

    int nofree = 0;
    SymbolTable* symbols;
private:
    void exit(std::string exit_message);
    static void exit(RuntimeException exception);

    Value* returned = nullptr;
    Node *current_node = nullptr;
};

bool cmp_any_int(Value* a, Value* b, const std::function<bool(int, int)>& comparison);


void printAST(Node* root);
