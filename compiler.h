#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.h"
#include "parser.h" 
#include "SymbolTable.h"

#define ISREPL 0

class Compiler {
public:
    Compiler(Lexer& lex) 
        : lexer(lex), 
          parser(lexer.read_file(), std::move(lexer.table)), 
          symbolTableStack(std::move(parser.symbolTableStack)) 
    {
        root = parser.parse();
    }

    void run();
    std::variant<double, std::string> evaluateAST(const ASTNodePtr& node);

private:
    Lexer& lexer;
    Parser parser;
    ASTNodePtr root;
    void REPL();
    std::stack<std::unique_ptr<SymbolTable>> symbolTableStack;
};

#endif // COMPILER_H
