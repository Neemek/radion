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

class DecimalValue : public Value {
public:
    explicit DecimalValue(double initial);

    double number;

    std::string to_string() override;
    bool equals(Value *other) override;
    Value * copy() override;
};

char digit_to_char(int digit);
