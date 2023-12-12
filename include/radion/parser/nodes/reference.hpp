#pragma once
#include "radion/parser/node.hpp"
#include <string>

struct ReferenceNode : Node {
    int type = NodeType::Reference;
    std::string name;
};
