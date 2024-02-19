#pragma once
#include <string>
#include "radion/parser/node.hpp"

struct LoopNode : Node {
    LoopNode() : Node(NodeType::Loop) {}
    Node* condition{};
    
    Node* doo{};
    Node* logic{};
};

struct RangeNode : Node {
    RangeNode() : Node(NodeType::Range) {}

    Node* from{};
    Node* to{};
    Node* increase{};
};

struct ForNode : Node {
    ForNode() : Node(NodeType::For) {}
    
    std::string counter;
    Node* values{};

    Node* logic{};
};
