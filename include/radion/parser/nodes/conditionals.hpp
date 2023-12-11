#include "radion/parser/node.hpp"

struct IfNode : Node {
    const int type = NodeType::If;
    Node condition;
    Node logic;

    Node otherwise;
};
