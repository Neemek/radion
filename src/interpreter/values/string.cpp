//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/values/string.hpp"

std::string StringValue::to_string() {
    return this->content;
}

StringValue::StringValue(std::string initial) : Value(ValueType::String) {
    this->content = std::move(initial);
}

bool StringValue::equals(Value *other) {
    return other->has_type(ValueType::String) && this->content == other->as<StringValue>()->content;
}
