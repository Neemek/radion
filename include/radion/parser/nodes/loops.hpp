#pragma once
#include "radion/parser/node.hpp"

struct LoopNode : Node {
    int type = NodeType::Loop;
    Node condition;
    
    Node logic;
};
