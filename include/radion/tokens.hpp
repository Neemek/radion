#pragma once

enum TokenType
{
    // Values
    ASSIGN,
    NAME,

    // Primitive types
    STRING,
    NUMBER,

    // Builtin Constants
    NIL,
    TRUE,
    FALSE,

    // Arithmetics
    PLUS,
    DOUBLE_PLUS,
    MINUS,
    DOUBLE_MINUS,
    STAR,
    SLASH,
    POWER,
    PERCENTAGE,

    // Truthyness
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    GREATER_THAN,
    GREATER_EQUAL,
    NOT,

    // Parenthasis
    OPEN_PAREN,     CLOSE_PAREN,
    OPEN_BRACKET,   CLOSE_BRACKET,
    OPEN_CURLY,     CLOSE_CURLY,

    // Keywords
    IF,
    ELSE,
    FOR,
    IN,
    WHILE,
    DO,
    FUNC,
    RETURN,

    // Other
    DOT,
    DOUBLE_DOT,
    COMMA,
    END,
};
