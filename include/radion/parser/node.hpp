
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
    Arithmetic,
};

struct Node {
    const int type = -1;
};
