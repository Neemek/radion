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
    Block,
    If,
    Call,
    Define,
    Arithmetic,
};

struct Node {
    Node(NodeType t) {
        type = t;
    }
    NodeType type;
};
