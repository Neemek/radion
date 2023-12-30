#pragma once
#include "radion/parser/node.hpp"

enum ArithmeticOperation {
    ADD,        SUBTRACT,
    MULTIPLY,   DIVIDE,
    MODULO,     EXPONENTIATION,
};


struct ArithmeticNode : Node {
    ArithmeticNode() : Node(NodeType::Arithmetic) {}

    ArithmeticOperation op;

    Node* left;
    Node* right;
};
