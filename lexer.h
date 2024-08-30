#ifndef Lexer_H

#define Lexer_H

#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include "token.h"

class Lexer {
    std::vector<TOKEN> read_file();
    bool check_if_number(std::string c);
    bool check_if_operator(std::string c);
};


#endif // Lexer_H