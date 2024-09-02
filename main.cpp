#include "compiler.h"
#include "lexer.h"
#include "parser.h"

int main()
{
    Lexer lexer;
    Compiler compiler(lexer);
    compiler.run();
}