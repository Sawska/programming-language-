#include "lexer.h"
#include "token.h"


std::vector<TOKEN> Lexer::read_file()
{
    std::string filename;

    getline(std::cin, filename);
    
    std::ifstream fileContent(filename);

    std::vector<TOKEN> result;
    if (!fileContent) // Check if file opened successfully
    {
        std::cerr << "Can't open the file" << std::endl;
        return result;
    }

    char c;

    while (fileContent.get(c))
    {
        TOKEN tok;
        if (check_if_number(c))
        {
            std::string number;
            number.push_back(c);
            while (fileContent.get(c) && check_if_number(c))
            {
                number.push_back(c);
            }

            tok.concept = TOKEN::TOKEN_CONCEPTS::NUMBER;
            tok.number = std::stoi(number);
            result.push_back(tok);
            if (fileContent) fileContent.unget();
        }
        else if (check_if_operator(c))
        {
            processOperator(c, fileContent, result);
        }
        else
        {
            
            std::cerr << "Unexpected character encountered: " << c << std::endl;
        }
    }

    if (fileContent.eof())
    {
        std::cout << "End of file reached" << std::endl;
    }
    else if (fileContent.fail())
    {
        std::cerr << "Error reading the file" << std::endl;
    }

    std::cout << "I'm here" << std::endl;
    fileContent.close();
    
    return result;
}



bool Lexer::check_if_number(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::check_if_operator(char c) {
    switch (c) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '&':
        case '|':
        case '=':
        case '!':
        case '?':
        case '~':
            return true;
        default:
            return false;
    }
}


void Lexer::processOperator(char c, std::ifstream &fileContent, std::vector<TOKEN> &result) {
    TOKEN tok;

    auto push_operator_token = [&result](TOKEN &tok, TOKEN::OPERATORS op) {
        tok.op = op;
        result.push_back(tok);
    };

    if (c == '+') {
        fileContent.get(c);
        push_operator_token(tok, (c == '+') ? TOKEN::OPERATORS::INCREMENT_OPERATOR : TOKEN::OPERATORS::PLUS_OPERATOR);
        if (c != '+') fileContent.unget();
    } else if (c == '-') {
        fileContent.get(c);
        push_operator_token(tok, (c == '-') ? TOKEN::OPERATORS::DECREMENT_OPERATOR : TOKEN::OPERATORS::MINUS_OPERATOR);
        if (c != '-') fileContent.unget();
    } else if (c == '&') {
        fileContent.get(c);
        push_operator_token(tok, (c == '&') ? TOKEN::OPERATORS::AND_OPERATOR : TOKEN::OPERATORS::BIT_AND_OPERATOR);
        if (c != '&') fileContent.unget();
    } else if (c == '|') {
        fileContent.get(c);
        push_operator_token(tok, (c == '|') ? TOKEN::OPERATORS::OR_OPERATOR : TOKEN::OPERATORS::BIT_OR_OPERATOR);
        if (c != '|') fileContent.unget();
    } else if (c == '*') {
        push_operator_token(tok, TOKEN::OPERATORS::MULTIPLY_OPERATOR);
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
            push_operator_token(tok, TOKEN::OPERATORS::DIVIDE_OPERATOR);
            fileContent.unget();
        }
    } else if (c == '!') {
        fileContent.get(c);
        push_operator_token(tok, (c == '=') ? TOKEN::OPERATORS::NOT_EQUALS_OPERATOR : TOKEN::OPERATORS::NOT_OPERATOR);
        if (c != '=') fileContent.unget();
    } else if (c == '~') {
        push_operator_token(tok, TOKEN::OPERATORS::BIT_NOT_OPERATOR);
    } else if (c == '>') {
        fileContent.get(c);
        if (c == '>') {
            fileContent.get(c);
            if (c == '=') {
                push_operator_token(tok, TOKEN::OPERATORS::RIGHT_SHIFT_EQUAL_OPERATOR);
            } else {
                
                push_operator_token(tok, TOKEN::OPERATORS::RIGHT_SHIFT_OPERATOR);
                fileContent.unget();
            }
        } else {
            push_operator_token(tok, (c == '=') ? TOKEN::OPERATORS::GREATER_THAN_EQUAL_OPERATOR : TOKEN::OPERATORS::GREATER_THAN_OPERATOR);
            if (c != '=') fileContent.unget();
        }
    } else if (c == '<') {
        fileContent.get(c);
        if (c == '<') {
            fileContent.get(c);
            if (c == '=') {
                push_operator_token(tok, TOKEN::OPERATORS::LEFT_SHIFT_EQUAL_OPERATOR);
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::LEFT_SHIFT_OPERATOR);
                fileContent.unget();
            }
        } else {
            push_operator_token(tok, (c == '=') ? TOKEN::OPERATORS::LESS_THAN_EQUAL_OPERATOR : TOKEN::OPERATORS::LESS_THAN_OPERATOR);
            if (c != '=') fileContent.unget();
        }
    } else if (c == '=') {
        fileContent.get(c);
        push_operator_token(tok, (c == '=') ? TOKEN::OPERATORS::COMPARE_OPERATOR : TOKEN::OPERATORS::EQUALS_OPERATOR);
        if (c != '=') fileContent.unget();
    } else if (c == '?') {
        push_operator_token(tok, TOKEN::OPERATORS::QUESTION_OPERATOR);
    } else if (c == '(') {
        tok.concept = TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS;
        result.push_back(tok);
    } else if (c == ')') {
        tok.concept = TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS;
        result.push_back(tok);
    }
}

