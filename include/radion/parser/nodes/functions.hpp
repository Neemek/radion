#pragma once
#include "radion/parser/node.hpp"
#include <string>

#define MAX_PARAMS 255

struct CallNode : Node {
    CallNode() : Node(NodeType::Call) {}
    CallNode(NodeType t) : Node(t) {}
    
    std::string name;
    Node* params[MAX_PARAMS];
};

struct DefineNode : CallNode {
    DefineNode() : CallNode(NodeType::Define) {}
    Node* logic;
    std::string params[MAX_PARAMS];
};
