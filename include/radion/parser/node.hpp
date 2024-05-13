#pragma once


enum NodeType {
    // literals
    IntLiteral,
    DecimalLiteral,
    StringLiteral,
    BooleanLiteral,
    NilLiteral,
    ListLiteral,

    // Loops
    Loop,
    For,

    // Variables
    Reference,
    Assign,
    Change,

    // Operation
    Arithmetic,
    Comparison,
    Not,

    // Conditional
    If,

    // Function
    Define,
    InlineDef,
    Return,
    Call,

    // Helpers
    Range,

    // Other
    Block,
};

struct Node {
    explicit Node(NodeType t) {
        type = t;
    }
    NodeType type;

    int start{};
    int end{};
};
