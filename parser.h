#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <memory>
#include "token.h"
#include "AST.h"
#include "SymbolTable.h"
#include <stack>
#include "StringNode.h"
#include "NumberNode.h"
#include "EmptyNode.h"
#include "VariableNode.h"
#include "UnaryOperationNode.h"
#include "BinaryOperationNode.h"
#include "BlockNode.h"
#include "ReturnNode.h"

class Parser {
public:
    Parser(Parser&&) = default; 
    Parser& operator=(Parser&&) = default; 
    
    
    Parser(const std::vector<TOKEN>& tokens, SymbolTable&& table)
        : tokens(tokens), index(0)  {
            symbolTableStack.push(std::make_unique<SymbolTable>(std::move(table)));
        }

    ASTNodePtr parse();
    ASTNodePtr parseExpression();
    ASTNodePtr parseTerm();
    ASTNodePtr parseFactor();
    ASTNodePtr parseLogicalExpression();
    ASTNodePtr parseVariableOrAssignment();
    ASTNodePtr handleVariableReference();
    ASTNodePtr parseBlock();
    ASTNodePtr parseStatement();
    ASTNodePtr parseIf();
    ASTNodePtr parseWhile();
    ASTNodePtr parseReturn();
    std::stack<std::unique_ptr<SymbolTable>> symbolTableStack;


private:
    std::vector<TOKEN> tokens;
    int index;
};

#endif // PARSER_H
