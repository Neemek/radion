#include "radion/parser/node.hpp"
#include <string>

struct IntLiteralNode : Node {
    int number;
};

struct StringLiteralNode : Node {
    std::string string;
};

struct BooleanLiteralNode : Node {
    bool boolean;
};

struct NilLiteralNode : Node {
};
