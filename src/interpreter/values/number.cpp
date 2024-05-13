//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/values/number.hpp"

#include <sstream>

IntValue::IntValue(int initial) : Value(ValueType::Int) {
    this->number = initial;
}

std::string IntValue::to_string() {
    return std::to_string(this->number);
}

bool IntValue::equals(Value *other) {
    if (other->has_type(ValueType::Float))
        return other->equals(this);

    return other->has_type(ValueType::Int) && this->number == other->as<IntValue>()->number;
}

Value *IntValue::copy() {
    return new IntValue(this->number);
}

FloatValue::FloatValue(float initial) : Value(ValueType::Float) {
    this->number = initial;
}

std::string FloatValue::to_string() {
    std::stringstream stream;

    stream << floor(this->number) << ".";
    float n = this->number;
    do {
        n *= 10;
        int digit = (int)n % 10;
        stream << digit_to_char(digit);
    } while (floor(n) != n);

    return stream.str();
}

char digit_to_char(int digit) {
    return '0' + digit;
}

bool FloatValue::equals(Value *other) {
    if (other->has_type(ValueType::Int))
        return this->number == (float)other->as<IntValue>()->number;

    return other->has_type(ValueType::Float) && other->as<FloatValue>()->number == this->number;
}

Value *FloatValue::copy() {
    return new FloatValue(this->number);
}
