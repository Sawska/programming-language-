#include "SymbolTable.h"

TOKEN SymbolTable::getVariableValue(const std::string& name) const {
    auto it = table.find(name);
    if (it != table.end()) {
        return it->second;
    }

    TOKEN notFoundToken;
    notFoundToken.op = TOKEN::OPERATORS::UNKNOWN;
    
 
    std::cout << "Variable not found: " << name << std::endl;
    std::cout << "Current table contents:" << std::endl;
    for (const auto& entry : table) {
        std::cout << "Name: " << entry.first << ", Value: " << entry.second.number << std::endl;
    }
    
    return notFoundToken;
}


void SymbolTable::setVariableValue(const std::string& name, TOKEN token) {
    std::cout << "setVariableValue" << std::endl;
    std::cout << name << std::endl;
    table[name] = token;

}