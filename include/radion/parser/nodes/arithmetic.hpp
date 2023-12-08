#include "radion/parser/node.hpp"

enum ArithmeticOperation {
    ADD,        SUBTRACT,
    MULTIPLY,   DIVIDE,
    MODULO,     EXPONENTIATION,
};


struct ArithmeticNode : Node {
    ArithmeticOperation op;

    Node left;
    Node right;
};
