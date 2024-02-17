#pragma once

#include <any>
#include <map>
#include <string>
#include "radion/interpreter/value.hpp"

class SymbolTable {
public:
    SymbolTable();
    SymbolTable(SymbolTable* super);

    Value* get(std::string name);
    void put(std::string name, Value* value);

    SymbolTable* sub();
    SymbolTable* super;
private:
    std::map<std::string, Value*> innerTable;
};
