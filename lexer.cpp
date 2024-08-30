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
        if(check_if_number(c+"")) {
            std::string number;
            number.push_back(c);
            while(check_if_number(c+""))
            {
                fileContent.get(c);
                number.push_back(c);
            }
            
            tok.concept =  TOKEN::TOKEN_CONCEPTS::NUMBER;
            tok.number = number;
             result.push_back(tok);
            

        } else if(check_if_operator(c+""))
        {
            if(c == '+')
            {
                fileContent.get(c);

                if(c == '+')
                {
                    tok.op = TOKEN::OPERATORS::INCREMENT_OPERATOR;
                }  else {
                    tok.op = TOKEN::OPERATORS::PLUS_OPERATOR;
                }
                result.push_back(tok);
            }  else if( c == '-')
            {
                fileContent.get(c);

                if(c == '-')
                {
                    tok.op = TOKEN::OPERATORS::DECREMENT_OPERATOR;
                } else {
                    tok.op = TOKEN::OPERATORS::MINUS_OPERATOR;
                }
                result.push_back(tok);       
            }
        } else if(c == '&')
        {
            fileContent.get(c);

            if( c == '&')
            {
                tok.op = TOKEN::OPERATORS::AND_OPERATOR;
            } else {
                tok.op = TOKEN::OPERATORS::BIT_AND_OPERATOR;
            }
            result.push_back(tok);       
        } else if(c == '|')
        {
            fileContent.get(c);

            if(c == '&')
            {
                tok.op = TOKEN::OPERATORS::OR_OPERATOR;
            }  else {
                tok.op = TOKEN::OPERATORS::BIT_OR_OPERATOR;
            }
            result.push_back(tok);
        } else if(c == '*')
        {
            tok.op = TOKEN::OPERATORS::MULTIPLY_OPERATOR;
            result.push_back(tok);
        } else if(c == '/')
        {
            tok.op = TOKEN::OPERATORS::DIVIDE_OPERATOR;
            result.push_back(tok);
        } else if(c == '!')
        {
            tok.op = TOKEN::OPERATORS::NOT_OPERATOR;
            result.push_back(tok);
        } else if(c == '~')
        {
            tok.op = TOKEN::OPERATORS::BIT_NOT_OPERATOR;
            result.push_back(tok);
        } else if( c == '>')
        {
            fileContent.get(c);

            if(c = '>')
            {
                tok.op = TOKEN::OPERATORS::LEFT_SHIT_OPERATOR;
            } else {
                tok.op = TOKEN::OPERATORS::BIGGER_OPERATOR;
            }
            result.push_back(tok);
        } else if(c == '<') {
            fileContent.get(c);

            if(c = '<')
            {
                tok.op = TOKEN::OPERATORS::RIGHT_SHIT_OPERATOR;
            } else {
                tok.op = TOKEN::OPERATORS::SMALLER_OPERATOR;
            }
            result.push_back(tok);
        } else if (c == '=')
        {
            fileContent.get(c);

            if(c == '=')
            {
                tok.op = TOKEN::OPERATORS::COMPARE_OPERATOR;
            } else {
                tok.op = TOKEN::OPERATORS::EQUALS_OPERATOR;
            }
            result.push_back(tok);
        } else if(c == '?')
        {
            tok.op = TOKEN::OPERATORS::QUESTION_OPERATOR;
            result.push_back(tok);
        } else if(c == '(')
        {
            tok.concept = TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE;
            result.push_back(tok);
        } else if(c == ')')
        {
            tok.concept = TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE;
            result.push_back(tok);
        } else if( c == '/')
        {
            fileContent.get(c);

            if(c == '/')
            {
                while (c != '\n')
                {
                    fileContent.get(c);
                }
                
            } else if(c == '*')
            {
                while (c)
                {
                    fileContent.get(c);

                    if(c == '*')
                    {
                        fileContent.get(c);
                        if(c == '/')
                        {
                            continue;
                        }
                    }
                }
                tok.concept = TOKEN::TOKEN_CONCEPTS::ERROR_TOKEN;
                result.push_back(tok);
                return result;
            } else {
                tok.concept = TOKEN::TOKEN_CONCEPTS::ERROR_TOKEN;
                result.push_back(tok);
                return result;
            }           
        } 
    }
    
    fileContent.close();
    return result;
}


bool Lexer:: check_if_number(std::string c) {
    std::regex e ("([0-9]");
    
    return std::regex_match(c,e);
}

bool Lexer::check_if_operator(std::string c)
{
    std::regex e ("[+-/*&|!=?~]");

    return std::regex_match(c,e);
}