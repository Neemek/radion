#include "radion/interpreter/symboltable.hpp"
#include "radion/parser/nodes/literal.hpp"


SymbolTable::SymbolTable() {
    this->super = nullptr;
}

SymbolTable::SymbolTable(SymbolTable* super) {
    this->super = super;
}

Value* SymbolTable::get(std::string name) {
    if (this->innerTable.count(name)) return this->innerTable[name];
    if (this->super == nullptr) return nullptr;

    return this->super->get(name);
}

void SymbolTable::put(std::string name, Value* value) {
    if (this->super != nullptr && this->super->get(name) != nullptr) this->super->put(name, value);
    else this->innerTable[name] = value;
}

SymbolTable* SymbolTable::sub() {
    return new SymbolTable(this);
}
