
enum NodeType {
    // literals
    IntLiteral,
    StringLiteral,
    BooleanLiteral,
    NilLiteral,

    // Other
    Reference,
    Loop,
    Block,
    If,
    Call,
    Define,
    Arithemtic,
};

struct Node {
    const int type = -1;
};
