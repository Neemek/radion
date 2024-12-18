//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//
#pragma once

#include "radion/interpreter/value.hpp"

class BooleanValue : public Value {
public:
    explicit BooleanValue(bool initial);
    std::string to_string() override;

    bool boolean;
    bool equals(Value *other) override;
    Value * copy() override;

    static BooleanValue *from(bool boolean);
};

static BooleanValue* BOOLEAN_TRUE = new BooleanValue(true);
static BooleanValue* BOOLEAN_FALSE = new BooleanValue(false);

class NilValue : public Value {
public:
    NilValue() : Value(ValueType::Nil) {};

    std::string to_string() override;
    bool equals(Value *other) override;
    Value * copy() override;
};

static NilValue* NIL_VALUE = new NilValue;

