#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <memory>
#include "AST.h"

class SymbolTable {
public:
    SymbolTable() = default;
    SymbolTable(SymbolTable&&) noexcept = default;
    SymbolTable& operator=(SymbolTable&&) noexcept = default;
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;

    std::unique_ptr<AST> getVariableValue(const std::string& name);
    void setVariableValue(const std::string& name, std::unique_ptr<AST> value);
    void listVariables() const;

    SymbolTable clone() const;

    std::unordered_map<std::string, std::unique_ptr<AST>> deepCopyTable() const;

    std::unordered_map<std::string, std::unique_ptr<AST>> table;
private:
};

#endif // SYMBOL_TABLE_H