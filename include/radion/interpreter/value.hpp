#pragma once
#include <string>
#include <vector>


enum ValueType {
    // Primitive
    Int,    Decimal,
    String, List,
    // defined
    Class,  Func,
    // Constant
    Nil,    Boolean,
};

class Value {
public:
    explicit Value(ValueType type);

    virtual std::string to_string()=0;
    virtual Value* copy()=0;
    virtual std::vector<Value*>::iterator iter();
    virtual bool equals(Value *other)=0;

    ValueType get_type();
    bool has_type(ValueType type);
    std::string get_typename();

    template <typename T>
    T* as() {
        return (T *)this;
    };

    Value* expect_type(ValueType type);
private:
    ValueType _type;
};

std::string value_type_name(ValueType type);
