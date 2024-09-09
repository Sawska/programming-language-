#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <map>
#include <string>
#include "token.h"
#include <iostream>
#include "AST.h"

class SymbolTable {
public:
    SymbolTable() = default;
    SymbolTable(SymbolTable&&) = default;
    SymbolTable& operator=(SymbolTable&&) = default;

        std::unordered_map<std::string, ASTNodePtr> table;

    
    ASTNodePtr& getVariableValue(const std::string& name);
    void setVariableValue(const std::string& name, ASTNodePtr value);
    void listVariables() const;
};

#endif //SYMBOL_TABLE_H
