#ifndef COMPILER_H

#define COMPILER_H
#include "parser.h"
#include "lexer.h"

#define ISREPL 0

class Compiler {
    public:
    Compiler(Lexer lex) : lexer(lex), parser(Parser(lexer.read_file())), root(parser.parse()) {}
    void run();
    private:
    Lexer lexer;
    Parser parser;
    ASTNodePtr root;
    void REPL();
    double evaluateAST(const ASTNodePtr& node);
    SymbolTable table;
};  

#endif //COMPILER_H