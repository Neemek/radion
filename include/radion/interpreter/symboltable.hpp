#pragma once

#include <any>
#include <map>
#include <string>

class SymbolTable {
public:
    SymbolTable();
    SymbolTable(SymbolTable* super);

    std::any get(std::string name);
    void put(std::string name, std::any value);

    SymbolTable* sub();
    SymbolTable* super;
private:
    std::map<std::string, std::any> innerTable;
};
