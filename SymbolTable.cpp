#include "SymbolTable.h"

ASTNodePtr SymbolTable::getVariableValue(const std::string& name) const {
    auto it = table.find(name);
    if (it != table.end()) {
        return it->second->clone();
    }

    std::cerr << "Variable not found: " << name << std::endl;
    return nullptr;
}

void SymbolTable::setVariableValue(const std::string& name, ASTNodePtr value) {
    table[name] = std::move(value);
}
