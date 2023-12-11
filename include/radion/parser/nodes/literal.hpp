#include "radion/parser/node.hpp"
#include <string>

#define NIL_VALUE NULL

struct IntLiteralNode : Node {
    const int type = NodeType::IntLiteral;
    int number;
};

struct StringLiteralNode : Node {
    const int type = NodeType::StringLiteral;
    std::string string;
};

struct BooleanLiteralNode : Node {
    const int type = NodeType::BooleanLiteral;
    bool boolean;
};

struct NilLiteralNode : Node {
    const int type = NodeType::NilLiteral;
};
