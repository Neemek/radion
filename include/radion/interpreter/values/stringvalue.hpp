#pragma once
#include "radion/interpreter/value.hpp"

class StringValue : public Value {
public:
    StringValue() : Value(ValueType::String) {};
    std::string to_string() override;

    std::string content;
};
