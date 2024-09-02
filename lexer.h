#ifndef Lexer_H

#define Lexer_H

#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include "token.h"

class Lexer {
    public:
    std::vector<TOKEN> read_file();
    private:
    void processOperator(char c, std::ifstream &fileContent, std::vector<TOKEN> &result);
    bool check_if_number(char c);
    bool check_if_operator(char c);
};


#endif // Lexer_H