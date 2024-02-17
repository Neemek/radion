//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include <sstream>
#include <utility>

#include "radion/interpreter/exceptions.hpp"

#include "radion/interpreter/value.hpp"
#include "radion/interpreter/values/string.hpp"
#include "radion/interpreter/values/number.hpp"
#include "radion/interpreter/values/constant.hpp"
#include "radion/interpreter/values/list.hpp"

Value::Value(ValueType type) {
    this->_type = type;
}

ValueType Value::get_type() {
    return this->_type;
}

std::string Value::get_typename() {
    return value_type_name(this->_type);
}

Value* Value::expect_type(ValueType type) {
    if (this->_type != type) throw new RuntimeException("expected type "+value_type_name(type)+", was "+ value_type_name(type));
    return this;
}

template<typename T>
T* *Value::as() {
    return (T *)this;
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

ListValue::ListValue(std::vector<Value *> elements) : Value(ValueType::List) {
    this->elements = std::move(elements);
}

std::string ListValue::to_string() {
    std::stringstream s;
    s << "[";

    int n = this->elements.size();
    for (int i = 0; i < n; i++) {
        s << this->elements.at(i)->to_string();
        if (i + 1 < n) s << ", ";
    }

    s << "]";
}


std::string NilValue::to_string() {
    return "nil";
}

std::string value_type_name(ValueType type) {
    switch (type) {
        case ValueType::Int:
            return "int";
        case ValueType::Float:
            return "float";
        case ValueType::Boolean:
            return "bool";
        case ValueType::String:
            return "string";
        case ValueType::Func:
            return "func";
        case ValueType::Class:
            return "class";
        case ValueType::Nil:
            return "nil";
    }
}
