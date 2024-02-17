//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/values/number.hpp"


IntValue::IntValue(int initial) : Value(ValueType::Int) {
    this->number = initial;
}

std::string IntValue::to_string() {
    return std::to_string(this->number);
}

FloatValue::FloatValue(float initial) : Value(ValueType::Float) {
    this->number = initial;
}

std::string FloatValue::to_string() {
    return std::to_string(this->number);
}
