//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//
#pragma once

#include "radion/interpreter/value.hpp"

class BooleanValue : public Value {
public:
    BooleanValue(bool initial);
    std::string to_string() override;

    bool boolean;
};

class NilValue : public Value {
public:
    NilValue() : Value(ValueType::Nil) {};

    std::string to_string() override;
};

