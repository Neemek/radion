//
// Created by Sindre Fagerheim Ødegaard on 17/02/2024.
//

#include "radion/interpreter/values/list.hpp"
#include <sstream>

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
