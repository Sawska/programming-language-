#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <memory>
#include "token.h"
#include "AST.h"
#include "SymbolTable.h"

class Parser {
public:
    Parser(Parser&&) = default; 
    Parser& operator=(Parser&&) = default; 
    
    
    Parser(const std::vector<TOKEN>& tokens, SymbolTable&& table)
        : tokens(tokens), index(0), table(std::move(table)) {}

    ASTNodePtr parse();
    ASTNodePtr parseExpression();
    ASTNodePtr parseTerm();
    ASTNodePtr parseFactor();
    ASTNodePtr parseLogicalExpression();
    ASTNodePtr parseVariableOrAssignment();
    ASTNodePtr handleVariableReference();
    SymbolTable table;

private:
    std::vector<TOKEN> tokens;
    int index;
};

#endif // PARSER_H
