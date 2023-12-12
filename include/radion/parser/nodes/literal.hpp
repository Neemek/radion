#pragma once
#include "radion/parser/node.hpp"
#include <string>

#define NIL_VALUE NULL

struct IntLiteralNode : Node {
    int type = NodeType::IntLiteral;
    int number;
};

struct StringLiteralNode : Node {
    int type = NodeType::StringLiteral;
    std::string string;
};

struct BooleanLiteralNode : Node {
    int type = NodeType::BooleanLiteral;
    bool boolean;
};

struct NilLiteralNode : Node {
    const int type = NodeType::NilLiteral;
};
