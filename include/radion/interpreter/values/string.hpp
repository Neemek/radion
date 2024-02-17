#pragma once
#include "radion/interpreter/value.hpp"

class StringValue : public Value {
public:
    StringValue(std::string initial);
    std::string to_string() override;

    std::string content;
};
