#include "radion/parser/node.hpp"

struct LoopNode : Node {
    const int type = NodeType::Loop;
    Node condition;
    
    Node logic;
};
