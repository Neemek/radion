#pragma once
#include "radion/parser/node.hpp"

enum ArithmeticOperation {
    ADD,        SUBTRACT,
    MULTIPLY,   DIVIDE,
    MODULO,     EXPONENTIATION,
};

std::string operation_to_symbol(ArithmeticOperation operation);

struct ArithmeticNode : Node {
    ArithmeticNode() : Node(NodeType::Arithmetic) {}

    ArithmeticOperation op=ADD;

    Node* left{};
    Node* right{};
};

struct ChangeNode : Node {
    ChangeNode() : Node(NodeType::Change) {}
    std::string name;
    bool ret_new = false;
    int changeBy = 1;
};
