#include "radion/parser/node.hpp"

struct LoopNode : Node {
    Node condition;
    
    Node logic;
};
