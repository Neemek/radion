#include "radion/parser/node.hpp"
#include <vector>

struct BlockNode : Node {
    const int type = NodeType::Block;
    std::vector<Node> statements;
};
