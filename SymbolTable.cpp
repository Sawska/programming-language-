#include "SymbolTable.h"

ASTNodePtr& SymbolTable::getVariableValue(const std::string& name) {
    auto it = table.find(name);
    if (it != table.end()) {
        return it->second; 
    }

    std::cerr << "Variable not found: " << name << std::endl;
    static ASTNodePtr empty_node = nullptr; 
    return empty_node;
}

void SymbolTable::setVariableValue(const std::string& name, ASTNodePtr value) {
    table[name] = std::move(value);  
}

void SymbolTable::listVariables() const {
    std::cout << "Current variables in the symbol table:" << std::endl;
    for (const auto& pair : table) {
        std::cout << "- " << pair.first << std::endl;  
    }
}
