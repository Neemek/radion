#pragma once

enum TokenType
{
    // Primitive types
    STRING,
    NUMBER,

    // Builtin Constants
    NIL,
    TRUE,
    FALSE,

    // Arithmetics
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,
    MODULO,

    // Truthyness
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    GREATER_THAN,
    GREATER_EQUAL,
    NOT,
};
