#pragma once
#include "radion/interpreter/value.hpp"

class StringValue : public Value {
public:
    explicit StringValue(std::string initial);
    std::string to_string() override;
    Value * copy() override;
    bool equals(Value* other) override;

    std::string content;
};
