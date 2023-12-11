#include "radion/parser/node.hpp"

enum ArithmeticOperation {
    ADD,        SUBTRACT,
    MULTIPLY,   DIVIDE,
    MODULO,     EXPONENTIATION,
};


struct ArithmeticNode : Node {
    const int type = NodeType::Arithemtic;
    ArithmeticOperation op;

    Node left;
    Node right;
};
