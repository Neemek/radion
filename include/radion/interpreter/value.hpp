#pragma once
#include <string>


enum ValueType {
    // Primitive
    Int,    Float,  String, List,
    // defined
    Class,  Func,
    // Constant
    Nil,    Boolean,
};

class Value {
public:
    explicit Value(ValueType type);
    virtual std::string to_string()=0;
    virtual bool equals(Value *other)=0;

    ValueType get_type();
    bool has_type(ValueType type);
    std::string get_typename();

    template <typename T>
    T* as();
    Value* expect_type(ValueType type);
private:
    ValueType _type;
};

std::string value_type_name(ValueType type);
