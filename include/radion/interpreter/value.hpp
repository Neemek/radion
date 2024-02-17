#pragma once
#include <string>


enum ValueType {
    // Primitive
    Int,    Float,  String,
    // defined
    Class,  Func,
    // Constant
    Nil,    Boolean,
};

class Value {
public:
    explicit Value(ValueType type);
    virtual std::string to_string()=0;

    ValueType get_type();
    std::string get_typename();
private:
    ValueType type;
};

std::string value_type_name(ValueType type);
