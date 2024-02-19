#pragma once


enum NodeType {
    // literals
    IntLiteral,
    StringLiteral,
    BooleanLiteral,
    NilLiteral,
    ListLiteral,

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

    int start;
    int end;
};
