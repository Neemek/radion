//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/value.hpp"
#include "radion/interpreter/values/stringvalue.hpp"

std::string StringValue::to_string() {
    return this->content;
}

