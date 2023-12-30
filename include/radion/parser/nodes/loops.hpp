#pragma once
#include "radion/parser/node.hpp"

struct LoopNode : Node {
    LoopNode() : Node(NodeType::Loop) {}
    Node* condition;
    
    Node* logic;
};
