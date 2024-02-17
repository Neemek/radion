#pragma once
#include <string>


enum ValueType {
    Int,    Float,  Boolean,
    String, Class,  Func,
};

class Value {
    ValueType type;
    
    virtual std::string to_string()=0;
};
