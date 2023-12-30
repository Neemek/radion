#pragma once
#include "radion/parser/node.hpp"

struct IfNode : Node {
    IfNode() : Node(NodeType::If) {}
    Node* condition;
    Node* logic;

    Node* otherwise;
};
