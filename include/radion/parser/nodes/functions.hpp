#include "radion/parser/node.hpp"
#include <string>

#define MAX_PARAMS 255

struct CallNode : Node {
    std::string name;
    Node params[MAX_PARAMS];
};

struct DefineNode : CallNode {
    Node logic;
};
