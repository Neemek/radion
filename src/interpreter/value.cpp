//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/exceptions.hpp"
#include "radion/interpreter/value.hpp"

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
    if (!this->has_type(type))
        throw RuntimeException("expected type "+value_type_name(type)+", was "+ value_type_name(this->_type));
    return this;
}

std::vector<Value *>::iterator Value::iter() {
    throw RuntimeException("Value is not iterable");
}

bool Value::has_type(ValueType type) {
    return this->get_type() == type;
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
        case ValueType::List:
            return "list";
    }
}
