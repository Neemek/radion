#include "radion/parser/node.hpp"
#include <string>

struct ReferenceNode : Node {
    const int type = NodeType::Reference;
    std::string name;
};
