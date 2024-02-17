#pragma once
#include "radion/interpreter/value.hpp"

class StringValue : public Value {
    ValueType type = ValueType::String;
    std::string content;

    std::string to_string() override;
};
