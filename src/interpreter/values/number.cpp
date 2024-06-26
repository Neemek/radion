//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/values/number.hpp"

#include <cmath>
#include <sstream>
#include <format>

IntValue::IntValue(int initial) : Value(ValueType::Int) {
    this->number = initial;
}

std::string IntValue::to_string() {
    return std::to_string(this->number);
}

bool IntValue::equals(Value *other) {
    if (other->has_type(ValueType::Decimal))
        return other->equals(this);

    return other->has_type(ValueType::Int) && this->number == other->as<IntValue>()->number;
}

Value *IntValue::copy() {
    return new IntValue(this->number);
}

DecimalValue::DecimalValue(double initial) : Value(ValueType::Decimal) {
    this->number = initial;
}

std::string DecimalValue::to_string() {
    // maybe later: https://lists.nongnu.org/archive/html/gcl-devel/2012-10/pdfkieTlklRzN.pdf
    auto str = std::to_string(this->number);

    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    // if the last character is a dot, add a zero for formatting
    if (str.at(str.size() - 1) == '.') str.append("0");

    return str;
}

bool DecimalValue::equals(Value *other) {
    if (other->has_type(ValueType::Int))
        return this->number == (double)other->as<IntValue>()->number;

    return other->has_type(ValueType::Decimal) && other->as<DecimalValue>()->number == this->number;
}

Value *DecimalValue::copy() {
    return new DecimalValue(this->number);
}
