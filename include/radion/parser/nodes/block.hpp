#include "radion/parser/node.hpp"
#include <vector>

struct BlockNode : Node {
    std::vector<Node> statements;
};
