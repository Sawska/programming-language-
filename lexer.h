#ifndef Lexer_H

#define Lexer_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "token.h"
#include <unordered_set>
#include <map>
#include "variable.h"
#include "SymbolTable.h"

class Lexer {
    public:
    Lexer() = default;
    Lexer(Lexer&&) = default;  
    Lexer& operator=(Lexer&&) = default; 
    std::vector<TOKEN> read_file();
    SymbolTable table;
    private:
    void processOperator(char c, std::ifstream &fileContent, std::vector<TOKEN> &result);
    bool check_if_number(char c);
    bool check_if_operator(char c);
    bool check_if_char(char c);
    void processChar(char c,std::ifstream &fileContent, std::vector<TOKEN> &result);
    void processVariable(std::ifstream &fileContent, std::vector<TOKEN> &result);
    void processString(char c,std::ifstream &fileContent, std::vector<TOKEN> &result);
    void processAssignment(std::ifstream &fileContent, std::vector<TOKEN> &result);
    std::unordered_set <std::string> private_words { "function" "let" "for" "while" "if" "else" "else if" "for", "class","break" "continue", "return"};
};

enum class LexerState {
    Start,
    Variable,
    While,
    IfStatment,
    elseStatment,
    VariableName,
    LetKeywoard,
    functionState,
    classState,
    forLoop,
    Assignment,
    Error,
    Break,
    Continue,
    Return,
    LetKeyword,
    ClassState,
    FunctionState,
    ElseStatement,
    IfStatement,
    ForLoop,
};


#endif // Lexer_H