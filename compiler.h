#ifndef COMPILER_H

#define COMPILER_H
#include <parser.h>
#include<lexer.h>

#define ISRELP 0;

class Compiler {
    Parser parser;
    Lexer lexer;
    ASTNodePtr root;
    void REPL();
    void run();
    double evaluateAST(const ASTNodePtr& node);
};  

#endif //COMPILER_H