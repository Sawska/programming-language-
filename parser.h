#ifndef Parser_H

#define Parser_H

#include <iostream>
#include <token.h>
#include <vector>
#include <AST.h>

class Parser {
    std::vector<TOKEN> tokens;
    int index;
    Parser(const std::vector<TOKEN>& tokens) : tokens(tokens), index(0)  {}
    
    ASTNodePtr parse();
    ASTNodePtr parseExpression();
    ASTNodePtr parseTerm();
    ASTNodePtr parseFactor();
    ASTNodePtr parseLogicalExpression();

    AST ast;
    
};

#endif //PARSER_H