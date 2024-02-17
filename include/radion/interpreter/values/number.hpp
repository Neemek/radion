//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//
#pragma once

#include "radion/interpreter/value.hpp"

class IntValue : public Value {
public:
    IntValue(int initial);

    int number;

    std::string to_string() override;
    bool equals(Value *other) override;
};

class FloatValue : public Value {
public:
    FloatValue(float initial);

    float number;

    std::string to_string() override;
    bool equals(Value *other) override;
};
