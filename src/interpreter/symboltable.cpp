#include "radion/interpreter/symboltable.hpp"
#include "radion/parser/nodes/literal.hpp"


SymbolTable::SymbolTable() {
    this->super = nullptr;
}

SymbolTable::SymbolTable(SymbolTable* super) {
    this->super = super;
}

std::any SymbolTable::get(std::string name) {
    if (this->innerTable.count(name)) return this->innerTable[name];
    if (this->super == nullptr) return std::any(); // return no value if at top

    return this->super->get(name);
}

void SymbolTable::put(std::string name, std::any value) {
    if (this->super != nullptr && this->super->get(name).has_value()) this->super->put(name, value);
    else this->innerTable[name] = value;
}

SymbolTable* SymbolTable::sub() {
    return new SymbolTable(this);
}
