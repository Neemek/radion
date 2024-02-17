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
private:
    ValueType type;
};
