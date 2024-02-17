//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//
#pragma once

#include "radion/interpreter/value.hpp"

class ListValue : public Value {
public:
    ListValue() : Value(ValueType::List) {};
    ListValue(std::vector<Value*> elements);

    std::vector<Value*> elements;

    std::string to_string() override;
};
