#include "radion/parser/node.hpp"
#include <string>

#define MAX_PARAMS 255

struct CallNode : Node {
    int type = NodeType::Call;
    std::string name;
    Node params[MAX_PARAMS];
};

struct DefineNode : CallNode {
    int type = NodeType::Define;
    Node logic;
};
