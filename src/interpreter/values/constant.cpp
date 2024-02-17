//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/values/constant.hpp"

BooleanValue::BooleanValue(bool initial) : Value(ValueType::Boolean) {
    this->boolean = initial;
}

std::string BooleanValue::to_string() {
    return this->boolean ? "true" : "false";
}

std::string NilValue::to_string() {
    return "nil";
}

