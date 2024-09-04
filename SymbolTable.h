#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <unordered_map>
#include <string>
#include "token.h"

class SymbolTable {
    public:
        std::unordered_map<std::string, TOKEN> table;
        TOKEN  getVariableValue(const std::string& name) const;
        void setVariableValue(const std::string& name, TOKEN token);
};

#endif //SYMBOL_TABLE_H