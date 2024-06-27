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

    unsigned int n = this->elements.size();
    for (unsigned int i = 0; i < n; i++) {
        s << this->elements.at(i)->to_string();
        if (i + 1 < n) s << ", ";
    }

    s << "]";
    return s.str();
}

bool ListValue::equals(Value *other) {
    if (!other->has_type(ValueType::List)) return false;
    auto *list = other->as<ListValue>();

    if (this->elements.size() != list->elements.size()) return false;

    for (unsigned int i = 0; i < this->elements.size(); ++i) {
        if (!this->elements.at(i)->equals(list->elements.at(i))) return false;
    }

    return true;
}

Value *ListValue::copy() {
    return new ListValue(this->elements);
}
