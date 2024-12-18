//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//
#pragma once

#include "radion/interpreter/value.hpp"
#include <vector>

class ListValue : public Value {
public:
    ListValue() : Value(ValueType::List) {};
    explicit ListValue(std::vector<Value*> elements);

    std::vector<Value*> elements;

    std::string to_string() override;
    bool equals(Value *other) override;
    Value * copy() override;
};
