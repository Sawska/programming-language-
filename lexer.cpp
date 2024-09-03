#include "lexer.h"
#include "token.h"


std::vector<TOKEN> Lexer::read_file() {
    std::string filename;
    getline(std::cin, filename);
    
    std::ifstream fileContent(filename);
    std::vector<TOKEN> result;

    if (!fileContent) {
        std::cerr << "Can't open the file" << std::endl;
        return result;
    }

    char c;
    while (fileContent.get(c)) {
        
        if (std::isspace(c)) {
            continue;
        }

        TOKEN tok;
        if (check_if_number(c)) {
            std::string number;
            number.push_back(c);
            while (fileContent.get(c) && check_if_number(c)) {
                number.push_back(c);
            }

            tok.concept = TOKEN::TOKEN_CONCEPTS::NUMBER;
            tok.number = std::stoi(number);
            result.push_back(tok);
            if (fileContent) fileContent.unget();
        }
        else if (check_if_operator(c)) {
            processOperator(c, fileContent, result);
        } else if(check_if_char(c))
        {

            processChar(c, fileContent, result);
        }
        else {
            std::cerr << "Unexpected character encountered: " << c << std::endl;
        }
    }

    if (fileContent.eof()) {
        std::cout << "End of file reached" << std::endl;
    }
    else if (fileContent.fail()) {
        std::cerr << "Error reading the file" << std::endl;
    }

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
        case '<':
        case '>':
        case '{':
        case '}':
        case '[':
        case ']':
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

    auto push_concept_token = [&result](TOKEN &tok, TOKEN::TOKEN_CONCEPTS concept) {
        tok.concept = concept;
        result.push_back(tok);
    };

    switch (c) {
        case '+':
            fileContent.get(c);
            if (c == '+') {
                push_operator_token(tok, TOKEN::OPERATORS::INCREMENT_OPERATOR);
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::PLUS_OPERATOR);
                if (c != '+') fileContent.unget();
            }
            break;
        case '-':
            fileContent.get(c);
            if (c == '-') {
                push_operator_token(tok, TOKEN::OPERATORS::DECREMENT_OPERATOR);
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::MINUS_OPERATOR);
                if (c != '-') fileContent.unget();
            }
            break;
        case '&':
            fileContent.get(c);
            if (c == '&') {
                push_operator_token(tok, TOKEN::OPERATORS::AND_OPERATOR);
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::BIT_AND_OPERATOR);
                if (c != '&') fileContent.unget();
            }
            break;
        case '|':
            fileContent.get(c);
            if (c == '|') {
                push_operator_token(tok, TOKEN::OPERATORS::OR_OPERATOR);
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::BIT_OR_OPERATOR);
                if (c != '|') fileContent.unget();
            }
            break;
        case '*':
            push_operator_token(tok, TOKEN::OPERATORS::MULTIPLY_OPERATOR);
            break;
        case '{':
            push_concept_token(tok,TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS);
        case '}':
            push_concept_token(tok,TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS);
        case '[':
            push_concept_token(tok,TOKEN::TOKEN_CONCEPTS::OPEN_SQUARE_BRACKETS);
        case ']':
            push_concept_token(tok,TOKEN::TOKEN_CONCEPTS::CLOSE_SQUARE_BRACKETS);
        case '/':
            fileContent.get(c);
            if (c == '/') {
                
                while (c != '\n' && fileContent.get(c)) {}
            } else if (c == '*') {
                
                while (fileContent.get(c)) {
                    if (c == '*' && fileContent.peek() == '/') {
                        fileContent.get(c); 
                        break;
                    }
                }
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::DIVIDE_OPERATOR);
                if (c != '*') fileContent.unget();
            }
            break;
        case '!':
            fileContent.get(c);
            if (c == '=') {
                push_operator_token(tok, TOKEN::OPERATORS::NOT_EQUALS_OPERATOR);
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::NOT_OPERATOR);
                if (c != '=') fileContent.unget();
            }
            break;
        case '~':
            push_operator_token(tok, TOKEN::OPERATORS::BIT_NOT_OPERATOR);
            break;
        case '>':
            fileContent.get(c);
            if (c == '>') {
                fileContent.get(c);
                if (c == '=') {
                    push_operator_token(tok, TOKEN::OPERATORS::RIGHT_SHIFT_EQUAL_OPERATOR);
                } else {
                    push_operator_token(tok, TOKEN::OPERATORS::RIGHT_SHIFT_OPERATOR);
                    if (c != '=') fileContent.unget();
                }
            } else if (c == '=') {
                push_operator_token(tok, TOKEN::OPERATORS::GREATER_THAN_EQUAL_OPERATOR);
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::GREATER_THAN_OPERATOR);
                if (c != '=') fileContent.unget();
            }
            break;
        case '<':
            fileContent.get(c);
            if (c == '<') {
                fileContent.get(c);
                if (c == '=') {
                    push_operator_token(tok, TOKEN::OPERATORS::LEFT_SHIFT_EQUAL_OPERATOR);
                } else {
                    push_operator_token(tok, TOKEN::OPERATORS::LEFT_SHIFT_OPERATOR);
                    if (c != '=') fileContent.unget();
                }
            } else if (c == '=') {
                push_operator_token(tok, TOKEN::OPERATORS::LESS_THAN_EQUAL_OPERATOR);
            } else {
                push_operator_token(tok, TOKEN::OPERATORS::LESS_THAN_OPERATOR);
                if (c != '=') fileContent.unget();
            }
            break;
        case '=':
            fileContent.get(c);
            push_operator_token(tok, (c == '=') ? TOKEN::OPERATORS::COMPARE_OPERATOR : TOKEN::OPERATORS::EQUALS_OPERATOR);
            if (c != '=') fileContent.unget();
            break;
        case '?':
            push_operator_token(tok, TOKEN::OPERATORS::QUESTION_OPERATOR);
            break;
        case '(':
            tok.concept = TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS;
            result.push_back(tok);
            break;
        case ')':
            tok.concept = TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS;
            result.push_back(tok);
            break;
        default:
            std::cerr << "Unexpected character encountered: " << c << std::endl;
    }
}

bool Lexer::check_if_char(char c) {
    return isalpha(c);
}

void Lexer::processChar(char c,std::ifstream &fileContent, std::vector<TOKEN> &result) {
    switch (c) {
        case '\'':
        case '"':
            processString(c,fileContent,result);
            break;
        default:
            break;
    }
    TOKEN tok;
    LexerState state = LexerState::Start;

    auto push_operator_token = [&result](TOKEN::TOKEN_CONCEPTS concept) {
        TOKEN tok;
        tok.concept = concept;
        result.push_back(tok);
    };

    auto get_next_char = [&fileContent]() -> char {
        char c;
        fileContent.get(c);
        return c;
    };
       std::string buffer;

    while(fileContent.good()) {
    switch(state) {
        case LexerState::Start:
            if (c == 'l') {
                state = LexerState::Variable;
            } else if (c == 'w') {
                state = LexerState::While;
            }  else if (c == 'f')
            {
                state = LexerState::forLoop;
            } else if(c == 'i')
            {
                state = LexerState::IfStatment;
            } else if (c == 'e')
            {
                state = LexerState::elseStatment;
            }  else if(c == 'f')
            {
                state = LexerState::functionState;
            } else if(c == 'c') {
                state = LexerState::classState;
            }
            else if(std::isalpha(c) || c == '_') {
                buffer.push_back(c);
                state = LexerState::VariableName;

            }
            else {
                state = LexerState::Error;
            }
            break;
        case LexerState::classState:
        if(c == 'l')
        {
            c = get_next_char();
            if(c == 'a')
            {
                c = get_next_char();
                if(c == 's')
                {
                    c = get_next_char();
                    if(c == 's')
                    {
                     // push class    
                    } else {
                        state = LexerState::Error;              
                    }
                } else {
                    state = LexerState::Error;          
                }
            } else {
                state = LexerState::Error;      
            }
        } else {
            state = LexerState::Error;  
        }

        case LexerState::functionState:
        if(c == 'u')
        {
            c = get_next_char();
            if(c == 'n')
            {
                c = get_next_char();
                if(c == 'c')
                {
                    c = get_next_char();
                    if(c == 't')
                    {
                        c = get_next_char();
                        if(c == 'i')
                        {
                            c = get_next_char();
                            if(c == 'o')
                            {
                                c = get_next_char();
                                if(c == 'n')
                                {
                                    // push function
                                }
                            } else {
                                state = LexerState::Error;                    
                            }
                        } else {
                            state = LexerState::Error;                
                        }
                    } else {
                        state = LexerState::Error;            
                    }
                } else {
                    state = LexerState::Error;        
                }
            } else {
                state = LexerState::Error;    
            }
        } else {
            state = LexerState::Error;
        }
        break;
        
        case LexerState::VariableName:
            if(std::isalnum(c) || c == '_') {
                buffer.push_back(c);
            } else {
                if(vars.find(buffer) != vars.end()) {
                    TOKEN varToken;
                    varToken.concept == TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME;
                    varToken.variableName = buffer;
                    result.push_back(varToken);
                } else {
                     throw std::runtime_error("Undefined variable: " + buffer);
                }
                buffer.clear();
                state = LexerState::Start;

                if(!fileContent.eof()) {
                    fileContent.unget();
                    c = get_next_char();
                    continue;
                }
            }
            break;
        case LexerState::elseStatment:
            if (c == 'l')
            {
                c = get_next_char();
                if( c == 's') {
                    c = get_next_char();
                    if(c == 'e')
                    {
                        // push else back
                    } else {
                    state = LexerState::Error;        
                    }
                } else {
                    state = LexerState::Error;
                }
            } else {
                state = LexerState::Error;
            }
        break;
        case LexerState::IfStatment:
            c = get_next_char();
            if( c ==  'f')
            {
                c = get_next_char();
                if(c == 'e')
                {   
                    c = get_next_char();
                    if(c == 'l')
                    {
                    c = get_next_char();
                    if(c == 's')
                    {
                        c = get_next_char();
                        if(c == 'e')
                        {
                            // push if else
                        } else {
                            state = LexerState::Error;
                        }
                    } else {
                        state = LexerState::Error;
                    } 
                    } else {
                        state = LexerState::Error;
                    }
                } else {
                    // push if statment
                }
            } else {
                state = LexerState::Error;
            }
            break;
        case LexerState::forLoop:
            c = get_next_char();
            if (c == 'o') {
                c = get_next_char();
                if(c == 'r')
                {

                } else {
                    state = LexerState::Error;
                }
            } else {
                state = LexerState::Error;
            }
            break;
        
        case LexerState::Variable:
            c = get_next_char();
            if (c == 'e') {
                c = get_next_char();
                if (c == 't') {
                    push_operator_token(TOKEN::TOKEN_CONCEPTS::VARIABLE);
                    state = LexerState::Start;
                } else {
                    state = LexerState::Error;
                }
            } else {
                state = LexerState::Error;
            }
            break;
             case LexerState::While:
                c = get_next_char();
                if (c == 'w') {
                    c = get_next_char();
                    if (c == 'h') {
                        c = get_next_char();
                        if (c == 'i') {
                            c = get_next_char();
                            if (c == 'l') {
                                c = get_next_char();
                                if (c == 'e') {
                                    
                                    state = LexerState::Start;
                                } else {
                                    state = LexerState::Error;
                                }
                            } else {
                                state = LexerState::Error;
                            }
                        } else {
                            state = LexerState::Error;
                        }
                    } else {
                        state = LexerState::Error;
                    }
                } else {
                    state = LexerState::Error;
                }
                break;
        
        case LexerState::Error:
            throw std::runtime_error("Unexpected character sequence");
        }

        if (state == LexerState::Start) {
            c = get_next_char();
        }
    }
}

void Lexer::processVariable(char c, std::ifstream &fileContent, std::vector<TOKEN> &result) {
    std::string variableName;

    
    TOKEN varToken;
    varToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE;
    result.push_back(varToken);

    
    while (fileContent.get(c)) {
        if (std::isspace(c) || c == '=') {
            break;
        }
        variableName += c;
    }

    
    if (std::find(private_words.begin(), private_words.end(), variableName) != private_words.end()) {
        throw std::runtime_error("Variable name cannot be a reserved keyword");
    }

    if (std::find(vars.begin(),vars.end(),variableName) != vars.end()) {
        throw std::runtime_error("Variable name cannot be a declared variable");
    }

    
    TOKEN varNameToken;
    varNameToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME;
    varNameToken.variableName = variableName;
    vars.insert(variableName);
    result.push_back(varNameToken);
    
    if (c == '=') {
        TOKEN assignToken;
        assignToken.op = TOKEN::OPERATORS::EQUALS_OPERATOR;
        result.push_back(assignToken);


        processAssignment( fileContent, result);
    } else {
        throw std::runtime_error("Expected '=' after variable declaration");
    }
}



void Lexer::processString(char c,std::ifstream &fileContent, std::vector<TOKEN> &result) {
    std::string strValue;
    char quoteChar = c;

    while (fileContent.get(c))  {
        if ( c == '\\') {
            fileContent.get(c);
            strValue += c;
        } else if( c == quoteChar) {
            break;
        } else {
            strValue +=c;
        }
    }
    TOKEN strToken;
    strToken.concept = TOKEN::TOKEN_CONCEPTS::STRING;
    strToken.string = strValue;
    result.push_back(strToken);
}


void Lexer::processAssignment(std::ifstream &fileContent, std::vector<TOKEN> &result) {
    char c;
    std::string expression;
    int prevLength = vars.size();

    while (fileContent.get(c)) {
        if (std::isspace(c)) {
            continue;
        }
        else if (c == '=') {
            
            while (fileContent.get(c)) {
            
                if (std::isspace(c) && (fileContent.peek() == '\n' || fileContent.peek() == EOF)) {
            
                    break;
                }
                expression.push_back(c);
            }

            
            TOKEN exprToken;
            exprToken.concept = TOKEN::TOKEN_CONCEPTS::EXPRESSION;
            exprToken.expression = expression;
            result.push_back(exprToken);
            if (expression.find("var")) {
                throw std::runtime_error("Assignment expression contains 'var' keyword");
            }

            return;
        }
        else {
            
            continue;
        }
    }


    throw std::runtime_error("Invalid assignment format or missing '='");
}
