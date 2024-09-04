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
        } else if(check_if_char(c) || c == '\'' || c == '"')
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

void Lexer::processChar(char c, std::ifstream &fileContent, std::vector<TOKEN> &result) {
    std::cout << "starting processing chars" << std::endl;
    switch (c) {
        case '\'':
        case '"':
            processString(c, fileContent, result);
            return;
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

    

    std::string buffer;

    while (fileContent.good()) {
        std::cout << c << std::endl;
        switch (state) {
            case LexerState::Start:
                if (c == 'l') {
                    state = LexerState::LetKeywoard;
                    fileContent.get(c);
                    continue;
                } else if (c == 'w') {
                    state = LexerState::While;
                    fileContent.get(c);;
                    continue;
                } else if (c == 'f') {
                    state = LexerState::forLoop;
                    fileContent.get(c);;
                    continue;
                } else if (c == 'i') {
                    state = LexerState::IfStatment;
                    fileContent.get(c);;
                    continue;
                } else if (c == 'e') {
                    state = LexerState::elseStatment;
                    fileContent.get(c);;
                    continue;
                } else if (c == 'f') {
                    state = LexerState::functionState;
                    fileContent.get(c);;
                    continue;
                } else if (c == 'c') {
                    state = LexerState::classState;
                    fileContent.get(c);;
                    continue;
                } else if (std::isalpha(c) || c == '_') {
                    buffer.push_back(c);
                    state = LexerState::VariableName;
                    fileContent.get(c);;
                    continue;
                } else {
                    state = LexerState::Error;
                }
                break;
                
            case LexerState::LetKeywoard:
            
            
                if (c == 'e') {
                    fileContent.get(c);
                            
                    if (c == 't') {
                        std::cout << c << std::endl;
                        state = LexerState::Start;
                        push_operator_token(TOKEN::TOKEN_CONCEPTS::VARIABLE);
                        processVariable(c,fileContent,result);
                        return;
                    } else {
                        state = LexerState::Error;
                    }
                } else {
                    state = LexerState::Error;
                }
                break;

            case LexerState::classState:
                if (c == 'l') {
                    fileContent.get(c);
                    if (c == 'a') {
                        fileContent.get(c);
                        if (c == 's') {
                            fileContent.get(c);
                            if (c == 's') {
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
                break;

            case LexerState::functionState:
                if (c == 'u') {
                    fileContent.get(c);
                    if (c == 'n') {
                        fileContent.get(c);
                        if (c == 'c') {
                            fileContent.get(c);
                            if (c == 't') {
                                fileContent.get(c);
                                if (c == 'i') {
                                    fileContent.get(c);
                                    if (c == 'o') {
                                        fileContent.get(c);
                                        if (c == 'n') {
                                            // push function
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
                } else {
                    state = LexerState::Error;
                }
                break;

            case LexerState::VariableName:
                if (std::isalnum(c) || c == '_') {
                    buffer.push_back(c);
                } else  if(isspace(c))
                {
                    while(isspace(c))
                    {
                        fileContent.get(c);;
                    }
                }
                else if (c == '=') {
                    std::cout << "in variable case" << std::endl;
                    TOKEN varToken;
                    varToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME;
                    varToken.variableName = buffer;
                    result.push_back(varToken);
                    TOKEN assignToken;
                    assignToken.op = TOKEN::OPERATORS::EQUALS_OPERATOR;
                    result.push_back(assignToken);
                    buffer.clear();
                    state = LexerState::Assignment;
                } else {
                    
                    if (table.getVariableValue(buffer).op == TOKEN::OPERATORS::UNKNOWN) {
                        TOKEN varToken;
                        varToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME;
                        varToken.variableName = buffer;
                        result.push_back(varToken);
                    } else {
                        throw std::runtime_error("Undefined variable: " + buffer);
                    }
                    buffer.clear();
                    state = LexerState::Start;
                    if (!fileContent.eof()) {
                        fileContent.unget();
                        fileContent.get(c);
                        continue;
                    }
                }
                break;

            case LexerState::Assignment:
                processAssignment(fileContent, result);
                return;
                break;

            case LexerState::elseStatment:
                if (c == 'l') {
                    fileContent.get(c);
                    if (c == 's') {
                        fileContent.get(c);
                        if (c == 'e') {
                            // push else
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
                fileContent.get(c);
                if (c == 'f') {
                    fileContent.get(c);
                    if (c == 'e') {
                        fileContent.get(c);
                        if (c == 'l') {
                            fileContent.get(c);
                            if (c == 's') {
                                fileContent.get(c);
                                if (c == 'e') {
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
                        // push if statement
                    }
                } else {
                    state = LexerState::Error;
                }
                break;

            case LexerState::forLoop:
                fileContent.get(c);
                if (c == 'o') {
                    fileContent.get(c);
                    if (c == 'r') {
                        // push for loop
                    } else {
                        state = LexerState::Error;
                    }
                } else {
                    state = LexerState::Error;
                }
                break;

            case LexerState::Variable:
                fileContent.get(c);
                if (c == 'e') {
                    fileContent.get(c);
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
                fileContent.get(c);
                if (c == 'w') {
                    fileContent.get(c);
                    if (c == 'h') {
                        fileContent.get(c);
                        if (c == 'i') {
                            fileContent.get(c);
                            if (c == 'l') {
                                fileContent.get(c);
                                if (c == 'e') {
                                    // push while loop
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
            fileContent.get(c);
        }
    }
}


void Lexer::processVariable(char c, std::ifstream &fileContent, std::vector<TOKEN> &result) {
    std::string variableName;


    TOKEN varToken;
    varToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE;
    result.push_back(varToken);

    while (std::isspace(c) && fileContent.get(c)) {}

    while (fileContent.get(c)) {
        std::cout << c << std::endl;
        if (std::isspace(c) || c == '=') {
            break;
        }
        variableName += c;
    }


    if (std::find(private_words.begin(), private_words.end(), variableName) != private_words.end()) {
        throw std::runtime_error("Variable name cannot be a reserved keyword");
    }


    if (table.getVariableValue(variableName).op != TOKEN::OPERATORS::UNKNOWN) {
        throw std::runtime_error("Variable name cannot be a declared variable");
    }


    TOKEN varNameToken;
    varNameToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME;
    varNameToken.variableName = variableName;
    result.push_back(varNameToken);
    table.setVariableValue(variableName, varNameToken);


    while (std::isspace(c) && fileContent.get(c)) {}


    
    if (c == '=') {
        TOKEN assignToken;
        assignToken.op = TOKEN::OPERATORS::EQUALS_OPERATOR;
        result.push_back(assignToken);

        processAssignment(fileContent, result);
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
    TOKEN tok;
    int prevLength = table.table.size();

    while (fileContent.get(c)) {
        if (std::isspace(c)) {
            continue; 
        }

        if (isalpha(c)) {
            
            processChar(c, fileContent, result);
        } else if (isdigit(c)) {
            
            std::string number;
            number.push_back(c);

            while (fileContent.get(c) && isdigit(c)) {
                number.push_back(c);
            }

            tok.concept = TOKEN::TOKEN_CONCEPTS::NUMBER;
            tok.number = std::stoi(number);
            result.push_back(tok);

            if (fileContent) fileContent.unget();
        } else {

            processOperator(c, fileContent, result);
        }


        if (c == '\n' || fileContent.peek() == EOF) {
            tok.op = TOKEN::OPERATORS::NEWLINE_OPERATOR;
            result.push_back(tok);
            break;
        }
    }

    if (table.table.size() != prevLength) {
        throw std::runtime_error("Cannot declare a variable within a variable assignment.");
    }
}
