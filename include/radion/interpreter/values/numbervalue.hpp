//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//
#pragma once

#include "radion/interpreter/value.hpp"

class IntValue : public Value {
    IntValue() : Value(ValueType::Int) {};

    int number;

    std::string to_string() override;
};

class FloatValue : public Value {
    FloatValue() : Value(ValueType::Float) {};

    float number;

    std::string to_string() override;
};
