#include "SymbolTable.h"
#include "AST.h"
#include <iostream>

std::unique_ptr<AST>& SymbolTable::getVariableValue(const std::string& name) {
    auto it = table.find(name);
    if (it != table.end()) {
        return it->second;
    }

    std::cerr << "Variable not found: " << name << std::endl;
    
    static std::unique_ptr<AST> empty_node = nullptr;
    return empty_node;
}

void SymbolTable::setVariableValue(const std::string& name, std::unique_ptr<AST> value) {
    table[name] = std::move(value);
}

void SymbolTable::listVariables() const {
    std::cout << "Current variables in the symbol table:" << std::endl;
    for (const auto& pair : table) {
        std::cout << "- " << pair.first << std::endl;
    }
}

std::unordered_map<std::string, std::unique_ptr<AST>> SymbolTable::deepCopyTable() const {
    std::unordered_map<std::string, std::unique_ptr<AST>> newTable;
    for (const auto& [key, value] : table) {
        newTable[key] = value ? value->clone() : nullptr;
    }
    return newTable;
}

SymbolTable SymbolTable::clone() const {
    SymbolTable cloned;
    cloned.table = deepCopyTable();
    return cloned;
}