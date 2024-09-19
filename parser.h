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
#include "IfNode.h"
#include "WhileNode.h"
#include "BreakNode.h"
#include "ContinueNode.h"
#include "ForNode.h"
#include "ArrayNode.h"
#include "ArrayAccessNode.h"
#include "FunctionNode.h"

class Parser {
public:
    Parser(Parser&&) = default; 
    Parser& operator=(Parser&&) = default; 
    
    
    
    Parser(const std::vector<TOKEN>& tokens, SymbolTable&& table,SymbolTable&& functionTable)
        : tokens(tokens), index(0)  {
            symbolTableStack.push(std::make_unique<SymbolTable>(std::move(table)));
            FunctionTableStack.push(std::make_unique<SymbolTable>(std::move(functionTable)));
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
    std::stack<std::unique_ptr<SymbolTable>> FunctionTableStack;
    ASTNodePtr findVariableInSymbolTableStack(const std::string& varName, SymbolTable& currentTable);
    ASTNodePtr parseContinue();
    ASTNodePtr parseBreak();
    ASTNodePtr parseFor();
    ASTNodePtr parseArray();
    ASTNodePtr parseFunction();
    ASTNodePtr handleFunctionRefrence();
    ASTNodePtr findFunctionInSymbolTableStack(const std::string& functionName, SymbolTable& currentTable);


private:
    std::vector<TOKEN> tokens;
    int index;
};

#endif // PARSER_H
