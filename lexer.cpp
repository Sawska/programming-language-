#include "lexer.h"
#include "token.h"


std::vector<TOKEN> Lexer::read_file()
{
    std::string filename;

    getline(std::cin,filename);
    
    std::ifstream fileContent;

    fileContent.open(filename);

    if(fileContent.fail())
    {
        std::cerr << "can't open the file" << std::endl;
        return;
    }

    char c;
    std::vector<TOKEN> result;

    while(fileContent.get(c))
    {
            TOKEN tok;
        if(check_if_number(c)) {
            std::string number;
            number.push_back(c);
            while(check_if_number(c))
            {
                fileContent.get(c);
                number.push_back(c);
            }
            
            tok.concept =  TOKEN::TOKEN_CONCEPTS::NUMBER;
            tok.number = number;
             result.push_back(tok);
            

        } else if(check_if_operator(c+"")) {
            processOperator(c,fileContent,result);
        }
    }
    
    fileContent.close();
    return result;
}


bool Lexer::check_if_number(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::check_if_operator(std::string c)
{
    std::regex e ("[+-/*&|!=?~]");

    return std::regex_match(c,e);
}

void Lexer::processOperator(char c, std::ifstream &fileContent, std::vector<TOKEN> &result) {
    TOKEN tok;

    auto push_operator_token = [&result](TOKEN &tok, TOKEN::OPERATORS op) {
        tok.op = op;
        result.push_back(tok);
    };

    if (c == '+') {
        fileContent.get(c);
        if (c == '+') {
            push_operator_token(tok, TOKEN::OPERATORS::INCREMENT_OPERATOR);
        } else {
            push_operator_token(tok, TOKEN::OPERATORS::PLUS_OPERATOR);
            fileContent.unget();
        }
    } else if (c == '-') {
        fileContent.get(c);
        if (c == '-') {
            push_operator_token(tok, TOKEN::OPERATORS::DECREMENT_OPERATOR);
        } else {
            push_operator_token(tok, TOKEN::OPERATORS::MINUS_OPERATOR);
            fileContent.unget();
        }
    } else if (c == '&') {
        fileContent.get(c);
        if (c == '&') {
            push_operator_token(tok, TOKEN::OPERATORS::AND_OPERATOR);
        } else {
            push_operator_token(tok, TOKEN::OPERATORS::BIT_AND_OPERATOR);
            fileContent.unget();
        }
    } else if (c == '|') {
        fileContent.get(c);
        if (c == '|') {
            push_operator_token(tok, TOKEN::OPERATORS::OR_OPERATOR);
        } else {
            push_operator_token(tok, TOKEN::OPERATORS::BIT_OR_OPERATOR);
            fileContent.unget();
        }
    } else if (c == '*') {
        push_operator_token(tok, TOKEN::OPERATORS::MULTIPLY_OPERATOR);
    } else if (c == '/') {
        push_operator_token(tok, TOKEN::OPERATORS::DIVIDE_OPERATOR);
    } else if (c == '!') {
        push_operator_token(tok, TOKEN::OPERATORS::NOT_OPERATOR);
    } else if (c == '~') {
        push_operator_token(tok, TOKEN::OPERATORS::BIT_NOT_OPERATOR);
    } else if (c == '>') {
        fileContent.get(c);
        if (c == '>') {
            push_operator_token(tok, TOKEN::OPERATORS::RIGHT_SHIT_OPERATOR);
        } else {
            push_operator_token(tok, TOKEN::OPERATORS::BIGGER_OPERATOR);
            fileContent.unget();
        }
    } else if (c == '<') {
        fileContent.get(c);
        if (c == '<') {
            push_operator_token(tok, TOKEN::OPERATORS::LEFT_SHIT_OPERATOR);
        } else {
            push_operator_token(tok, TOKEN::OPERATORS::SMALLER_OPERATOR);
            fileContent.unget();
        }
    } else if (c == '=') {
        fileContent.get(c);
        if (c == '=') {
            push_operator_token(tok, TOKEN::OPERATORS::COMPARE_OPERATOR);
        } else {
            push_operator_token(tok, TOKEN::OPERATORS::EQUALS_OPERATOR);
            fileContent.unget();
        }
    } else if (c == '?') {
        push_operator_token(tok, TOKEN::OPERATORS::QUESTION_OPERATOR);
    } else if (c == '(') {
        tok.concept = TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS;
        result.push_back(tok);
    } else if (c == ')') {
        tok.concept = TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS;
        result.push_back(tok);
    } else if (c == '/') {
        
        fileContent.get(c);
        if (c == '/') {
            while (c != '\n' && fileContent.get(c)) {}
        } else if (c == '*') {
            while (fileContent.get(c)) {
                if (c == '*') {
                    fileContent.get(c);
                    if (c == '/') break;
                }
            }
        } else {
            tok.concept = TOKEN::TOKEN_CONCEPTS::ERROR_TOKEN;
            result.push_back(tok);
            fileContent.unget();
        }
    }
}
