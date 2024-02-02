#pragma once
#include "radion/parser/node.hpp"
#include <string>

struct ReferenceNode : Node {
    ReferenceNode() : Node(NodeType::Reference) {}
    std::string name;
};

struct AssignNode : Node {
    AssignNode() : Node(NodeType::Assign) {}
    std::string name;
    Node* value;
};
