//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/value.hpp"
#include "radion/interpreter/values/stringvalue.hpp"
#include "radion/interpreter/values/numbervalue.hpp"

std::string StringValue::to_string() {
    return this->content;
}


std::string IntValue::to_string() {
    return std::to_string(this->number);
}

std::string FloatValue::to_string() {
    return std::to_string(this->number);
}
