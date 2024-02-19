#pragma once
#include "radion/parser/node.hpp"
#include <string>

#define MAX_PARAMS 255

struct CallNode : Node {
    CallNode() : Node(NodeType::Call) {}
    explicit CallNode(NodeType t) : Node(t) {}
    
    std::string name;
    std::vector<Node*> params;
};

struct DefineNode : CallNode {
    DefineNode() : CallNode(NodeType::Define) {}
    explicit DefineNode(NodeType t) : CallNode(t) {}
    Node* logic;
    std::vector<std::string> params;
};

struct ReturnNode : Node {
    ReturnNode() : Node(NodeType::Return) {}
    Node* value;
};

struct InlineDefNode : DefineNode {
    InlineDefNode() : DefineNode(NodeType::InlineDef) {}
};
