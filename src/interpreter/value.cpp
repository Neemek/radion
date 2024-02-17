//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include <utility>

#include "radion/interpreter/value.hpp"
#include "radion/interpreter/values/string.hpp"
#include "radion/interpreter/values/number.hpp"
#include "radion/interpreter/values/constant.hpp"

Value::Value(ValueType type) {
    this->type = type;
}

ValueType Value::get_type() {
    return this->type;
}


std::string StringValue::to_string() {
    return this->content;
}

StringValue::StringValue(std::string initial) : Value(ValueType::String) {
    this->content = std::move(initial);
}


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


BooleanValue::BooleanValue(bool initial) : Value(ValueType::Boolean) {
    this->boolean = initial;
}

std::string BooleanValue::to_string() {
    return this->boolean ? "true" : "false";
}


std::string NilValue::to_string() {
    return "nil";
}
