#pragma once
#include "radion/parser/node.hpp"
#include <vector>

struct BlockNode : Node {
    BlockNode() : Node(NodeType::Block) {}
    std::vector<Node*> statements;
};
