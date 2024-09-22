#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.h"
#include "parser.h" 
#include "SymbolTable.h"
#include "vector"

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
using ASTResult = std::variant<double, std::string, ContinueType, BreakType, ReturnType,VoidType,bool,std::vector<ASTNodePtr>>;
class Compiler {
public:
    Compiler(Lexer& lex) 
        : lexer(lex), 
          parser(lexer.read_file(), std::move(lexer.table),std::move(lexer.function_table),std::move(lexer.class_table))
    {
        root = parser.parse();
        symbolTableStack =  std::move(parser.symbolTableStack);
        functionTableStack = std::move(parser.FunctionTableStack);
        classTableStack = std::move(parser.classTableStack);
    }

    void run();
    ASTResult evaluateAST(const ASTNodePtr& node);
    void update_variable(const std::string& varName, ASTNodePtr node);
    ASTNodePtr find_variable_value(const std::string& varName);

private:
    Lexer& lexer;
    Parser parser;
    ASTNodePtr root;
    void REPL();
    ASTNodePtr findVariableInSymbolTableStack(const std::string& varName, SymbolTable& currentTable);
    std::stack<std::unique_ptr<SymbolTable>> symbolTableStack;
    std::stack<std::unique_ptr<SymbolTable>> functionTableStack;
    std::stack<std::unique_ptr<SymbolTable>> classTableStack;
    std::stack<bool> loopStack; 
};

#endif // COMPILER_H
