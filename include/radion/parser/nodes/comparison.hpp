#pragma once
#include "radion/parser/node.hpp"

enum ComparisonType {
    Equals,
    NotEquals,
    Greater,
    GreaterOrEqual,
    Less,
    LessOrEqual,
    //RoughlyEqual,
};

struct ComparisonNode : Node {
    ComparisonNode() : Node(NodeType::Comparison) {}
    Node* left{};
    Node* right{};

    ComparisonType comparison;
};

struct NotNode : Node {
    NotNode() : Node(NodeType::Not) {}
    Node* value{};
};

