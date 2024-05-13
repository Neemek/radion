#pragma once
#include "radion/parser/node.hpp"
#include <string>
#include <vector>

struct IntLiteralNode : Node {
    IntLiteralNode() : Node(NodeType::IntLiteral) {}
    int number{};
};

struct DecimalLiteralNode : Node {
    DecimalLiteralNode() : Node(NodeType::DecimalLiteral) {}
    float number{};
};

struct StringLiteralNode : Node {
    StringLiteralNode() : Node(NodeType::StringLiteral) {}
    std::string string;
};

struct BooleanLiteralNode : Node {
    BooleanLiteralNode() : Node(NodeType::BooleanLiteral) {}
    bool boolean{};
};

struct NilLiteralNode : Node {
    NilLiteralNode() : Node(NodeType::NilLiteral) {}
};

struct ListLiteralNode : Node {
    ListLiteralNode() : Node(NodeType::ListLiteral) {}
    std::vector<Node*> elements;
};
