#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <memory>
#include "token.h"
#include "AST.h"

class Parser {
public:
    Parser(const std::vector<TOKEN>& tokens) 
        : tokens(tokens), index(0) {}

    ASTNodePtr parse();
    ASTNodePtr parseExpression();
    ASTNodePtr parseTerm();
    ASTNodePtr parseFactor();
    ASTNodePtr parseLogicalExpression();

private:
    std::vector<TOKEN> tokens;
    int index;
    
};

#endif // PARSER_H
