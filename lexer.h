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

class Lexer {
    public:
    std::vector<TOKEN> read_file();
    private:
    void processOperator(char c, std::ifstream &fileContent, std::vector<TOKEN> &result);
    bool check_if_number(char c);
    bool check_if_operator(char c);
    bool check_if_char(char c);
    void processChar(char c,std::ifstream &fileContent, std::vector<TOKEN> &result);
    void processVariable(char c,std::ifstream &fileContent, std::vector<TOKEN> &result);
    void processString(char c,std::ifstream &fileContent, std::vector<TOKEN> &result);
    void processAssignment(std::ifstream &fileContent, std::vector<TOKEN> &result);
    void handleWhile(std::ifstream &fileContent, std::vector<TOKEN> &result);
    void handleClass(std::ifstream &fileContent, std::vector<TOKEN> &result);
    void handleForLoop(std::ifstream &fileContent, std::vector<TOKEN> &result);
    void handleIfStatment(std::ifstream &fileContent, std::vector<TOKEN> &result);
    void handleElseStatment(std::ifstream &fileContent, std::vector<TOKEN> &result);
    std::unordered_set <std::string> private_words { "function" "let" "for" "while" "if" "else" "else if" "for", "class"};
    std::unordered_set <std::string> vars;
};

enum class LexerState {
    Start,
    Variable,
    While,
    IfStatment,
    elseStatment,
    elseifStatment,
    VariableName,
    functionState,
    classState,
    forLoop,
    Error,
};


#endif // Lexer_H