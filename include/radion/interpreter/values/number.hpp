//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//
#pragma once

#include "radion/interpreter/value.hpp"

class IntValue : public Value {
public:
    explicit IntValue(int initial);

    int number;

    std::string to_string() override;
    bool equals(Value *other) override;
    Value * copy() override;
};

class FloatValue : public Value {
public:
    explicit FloatValue(float initial);

    float number;

    std::string to_string() override;
    bool equals(Value *other) override;
    Value * copy() override;
};
