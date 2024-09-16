#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.h"
#include "parser.h" 
#include "SymbolTable.h"

struct ContinueType {

};

struct VoidType {

};

struct BreakType {
    
};

struct ReturnType {
  
    ASTNodePtr returnValue;
    ReturnType(ASTNodePtr value) : returnValue(std::move(value)) {}
};

#define ISREPL 0
using ASTResult = std::variant<double, std::string, ContinueType, BreakType, ReturnType,VoidType>;
class Compiler {
public:
    Compiler(Lexer& lex) 
        : lexer(lex), 
          parser(lexer.read_file(), std::move(lexer.table))
    {
        root = parser.parse();
        symbolTableStack =  std::move(parser.symbolTableStack);
    }

    void run();
    ASTResult evaluateAST(const ASTNodePtr& node);
    void update_variable(const std::string& varName, ASTNodePtr node);

private:
    Lexer& lexer;
    Parser parser;
    ASTNodePtr root;
    void REPL();
    std::stack<std::unique_ptr<SymbolTable>> symbolTableStack;
    std::stack<bool> loopStack; 
};

#endif // COMPILER_H
