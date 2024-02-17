//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//
#pragma once

#include "radion/interpreter/value.hpp"

class IntValue : public Value {
    ValueType type = ValueType::Int;

    int number;

    std::string to_string() override;
};

class FloatValue : public Value {
    ValueType type = ValueType::Float;

    float number;

    std::string to_string() override;
};
