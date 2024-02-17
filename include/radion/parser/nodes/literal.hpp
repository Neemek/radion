#pragma once
#include "radion/parser/node.hpp"
#include <string>

struct IntLiteralNode : Node {
    IntLiteralNode() : Node(NodeType::IntLiteral) {}
    int number;
};

struct StringLiteralNode : Node {
    StringLiteralNode() : Node(NodeType::StringLiteral) {}
    std::string string;
};

struct BooleanLiteralNode : Node {
    BooleanLiteralNode() : Node(NodeType::BooleanLiteral) {}
    bool boolean;
};

struct NilLiteralNode : Node {
    NilLiteralNode() : Node(NodeType::NilLiteral) {}
};
