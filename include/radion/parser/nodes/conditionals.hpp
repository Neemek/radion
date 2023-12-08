#include "radion/parser/node.hpp"

struct IfNode : Node {
    Node condition;
    Node logic;

    Node otherwise;
};
