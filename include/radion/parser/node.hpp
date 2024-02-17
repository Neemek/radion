#pragma once


enum NodeType {
    // literals
    IntLiteral,
    StringLiteral,
    BooleanLiteral,
    NilLiteral,

    // Other
    Reference,
    Assign,
    Loop,
    Range,
    For,
    Block,
    If,
    Call,
    Define,
    InlineDef,
    Return,
    Arithmetic,
    Change,
    Comparison,
    Not,
};

struct Node {
    Node(NodeType t) {
        type = t;
    }
    NodeType type;
};
