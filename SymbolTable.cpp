#include "SymbolTable.h"

TOKEN SymbolTable::getVariableValue(const std::string& name) const {
    auto it = table.find(name);
    if (it != table.end()) {
        return it->second;
    }
    
    TOKEN notFoundToken;
    notFoundToken.op = TOKEN::OPERATORS::UNKNOWN; 
    return notFoundToken;
}


void SymbolTable::setVariableValue(const std::string& name, TOKEN token) {
    table[name] = token;
}