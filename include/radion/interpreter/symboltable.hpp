#pragma once

#include <map>
#include <string>
#include "radion/interpreter/value.hpp"

class SymbolTable {
public:
    SymbolTable();
    explicit SymbolTable(SymbolTable* super);

    Value* get(std::string name);
    void put(std::string name, Value* value);

    SymbolTable* sub();
    SymbolTable* super;
private:
    std::map<std::string, Value*> innerTable;
};
