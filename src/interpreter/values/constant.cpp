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

bool BooleanValue::equals(Value *other) {
    return other->has_type(ValueType::Boolean) && this->boolean == other->as<BooleanValue>()->boolean;
}

Value *BooleanValue::copy() {
    return this->boolean ? BOOLEAN_TRUE : BOOLEAN_FALSE;
}

std::string NilValue::to_string() {
    return "nil";
}

bool NilValue::equals(Value *other) {
    return other->has_type(ValueType::Nil);
}

Value *NilValue::copy() {
    return NIL_VALUE;
}
