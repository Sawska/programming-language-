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
          table(std::move(parser.table)) 
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
    SymbolTable table;
};

#endif // COMPILER_H
