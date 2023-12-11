#include "radion/parser/node.hpp"
#include <vector>

struct BlockNode : Node {
    int type = NodeType::Block;
    std::vector<Node> statements;
};
