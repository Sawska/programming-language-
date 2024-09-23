#include "lexer.h"
#include "token.h"


std::vector<TOKEN> Lexer::read_file() {
    std::string filename = "/Users/alexanderkorzh/Desktop/coding/programming_language/example.eli";
    
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

            
            if (fileContent) {
                fileContent.unget();
            } else {
                std::cerr << "Error: Unable to unget character after number" << std::endl;
            }
        }
        else if (check_if_operator(c)) {
            processOperator(c, fileContent, result);
            if(!fileContent)
            {
                break;
            }
        }
        else if (check_if_char(c)) {
            processChar(c, fileContent, result);
            if(!fileContent)
            {
                break;
            }
        }
        else if (c == '\'' || c == '"') {
            processString(c, fileContent, result);
            if(!fileContent)
            {
                break;
            }
        }
        else {
            std::cerr << "Unexpected character encountered: " << c << std::endl;
        }

        
        if (fileContent.eof()) {
            std::cout << "End of file reached" << std::endl;
        } else if (fileContent.fail()) {
            
            std::cerr << "Error reading the file" << std::endl;
            break;
        }
    }


    if (fileContent.fail() && !fileContent.eof()) {
        
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
        case '(':
        case ')':
        case ';':
        case ',':
        case ':':
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
            break;
        case '}':
            push_concept_token(tok,TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS);
            break;
        case '[':
            push_concept_token(tok,TOKEN::TOKEN_CONCEPTS::OPEN_SQUARE_BRACKETS);
            break;
        case ']':
            push_concept_token(tok,TOKEN::TOKEN_CONCEPTS::CLOSE_SQUARE_BRACKETS);
            break;
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
            tok.concept = TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS;
            result.push_back(tok);
            break;
        case ')':
            tok.concept = TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE_BRACKETS;
            result.push_back(tok);
            break;
        case ';':
            tok.concept = TOKEN::TOKEN_CONCEPTS::SEMICOLON;
            result.push_back(tok);
            break;
        case ',':
            tok.concept = TOKEN::TOKEN_CONCEPTS::COMMA;
            result.push_back(tok);
            break;
        case ':':
        tok.concept = TOKEN::TOKEN_CONCEPTS::EXTENDS;
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
    std::cout << "Starting processing chars" << std::endl;
    LexerState state = LexerState::Start;

    auto push_operator_token = [&result](TOKEN::OPERATORS op) {
        TOKEN tok;
        tok.op = op;
        result.push_back(tok);
    };

    auto push_concept_token = [&result](TOKEN::TOKEN_CONCEPTS concept) {
        TOKEN tok;
        tok.concept = concept;
        result.push_back(tok);
    };

    std::string buffer;
    buffer.push_back(c);

    
    

     while (fileContent.get(c) && (std::isalnum(c) || c == '_')) {
    buffer.push_back(c);
}


    
    if (buffer == "let") {
        state = LexerState::Start;
        buffer.clear();
        processVariable(fileContent, result);
        return;
    } 
    else if (buffer == "return") {
    push_concept_token(TOKEN::TOKEN_CONCEPTS::RETURN);
    return;

    } else if (buffer == "function") {
        TOKEN token;
        push_concept_token(TOKEN::TOKEN_CONCEPTS::FUNCTION);    
        buffer.clear();
        while( fileContent.get(c) && isspace(c)) {}
        fileContent.unget();
        while (fileContent.get(c) && (std::isalnum(c) || c == '_')) {
            buffer.push_back(c);
        }
        fileContent.unget();
        token.variableName = buffer;
        token.concept = TOKEN::TOKEN_CONCEPTS::FUNCTION_NAME;
        result.push_back(token);
        return;
    } else if(buffer == "for")  {
    push_concept_token(TOKEN::TOKEN_CONCEPTS::FOR);    
    fileContent.unget();
    return;
    } else if(buffer == "while") {
    push_concept_token(TOKEN::TOKEN_CONCEPTS::WHILE);    
    return;
    } else if (buffer == "if") {
    push_concept_token(TOKEN::TOKEN_CONCEPTS::IF);
    return;
}  else if(buffer == "true")
{
    push_concept_token(TOKEN::TOKEN_CONCEPTS::TRUE);
} else if(buffer == "false") {
    push_concept_token(TOKEN::TOKEN_CONCEPTS::FALSE);
}
else if (buffer == "else") {
    push_concept_token(TOKEN::TOKEN_CONCEPTS::ELSE);
    return;
    } 
    else if(buffer == "class") {
        TOKEN token;
        push_concept_token(TOKEN::TOKEN_CONCEPTS::CLASS);
        while (fileContent.get(c) && (std::isalnum(c) || c == '_')) {
        buffer.push_back(c);
        }

        token.variableName = buffer;
        token.concept = TOKEN::TOKEN_CONCEPTS::CLASS_NAME;
        buffer.clear();
        result.push_back(token);
    } else if(buffer == "new")
    {
        TOKEN token;
        push_concept_token(TOKEN::TOKEN_CONCEPTS::NEW);
        while (fileContent.get(c) && (std::isalnum(c) || c == '_')) {
        buffer.push_back(c);
        }

        token.variableName = buffer;
        token.concept = TOKEN::TOKEN_CONCEPTS::CLASS_NAME;
        buffer.clear();
        result.push_back(token);
    }
    else if (buffer == "break") {
        TOKEN tok;
        push_concept_token(TOKEN::TOKEN_CONCEPTS::BREAK);
        if(c == '\n')
        {
            push_operator_token(TOKEN::OPERATORS::NEWLINE_OPERATOR);
        }
        return;
    } else if (buffer == "continue") {
        TOKEN tok;
        push_concept_token(TOKEN::TOKEN_CONCEPTS::CONTINUE);
        if(c == '\n')
        {
            push_operator_token(TOKEN::OPERATORS::NEWLINE_OPERATOR);
        } 
        return;
    } else {
        state = LexerState::VariableName;
    }

    
    if (state == LexerState::VariableName) {
        std::cout << "Processing variable name" << std::endl;
        
        if (buffer.empty()) {
            throw std::runtime_error("Unexpected empty variable name");
        }

        TOKEN varToken;
        varToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME;
        varToken.variableName = buffer;
        result.push_back(varToken);
        buffer.clear();
        fileContent.unget();
        if (fileContent.get(c) && c == '=') {
            std::cout << "Processing assignment operator" << std::endl;
            TOKEN assignToken;
            assignToken.op = TOKEN::OPERATORS::EQUALS_OPERATOR;
            result.push_back(assignToken);
            processAssignment(fileContent, result);
        } else if (check_if_operator(c)) {
            processOperator(c, fileContent, result);
        } else {
            fileContent.unget();
        }
    }
}







void Lexer::processVariable(std::ifstream &fileContent, std::vector<TOKEN> &result) {
    std::string variableName;
    char c;

    TOKEN varToken;
    varToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE;
    result.push_back(varToken);

    while (fileContent.get(c) && std::isspace(c)) {}

    if (fileContent.eof() || !fileContent.good()) {
        std::cerr << "Error: Unexpected end of file or read failure while skipping whitespace" << std::endl;
        return;
    }

    do {
        if (std::isspace(c) || c == '=') {
            break;
        }
        variableName += c;
    } while (fileContent.get(c));
    
    if (std::find(private_words.begin(), private_words.end(), variableName) != private_words.end()) {
        throw std::runtime_error("Variable name cannot be a reserved keyword");
    }

    

    TOKEN varNameToken;
    varNameToken.concept = TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME;
    varNameToken.variableName = variableName;
    result.push_back(varNameToken);

    while (fileContent.get(c) && std::isspace(c)) {}

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

        if (std::isalpha(c)) {
            processChar(c, fileContent, result);
        } else if (std::isdigit(c)) {
            std::string number;
            number.push_back(c);

            while (fileContent.get(c) && std::isdigit(c)) {
                number.push_back(c);
            }

            tok.concept = TOKEN::TOKEN_CONCEPTS::NUMBER;
            tok.number = std::stoi(number);
            result.push_back(tok);
            if (fileContent) fileContent.unget();
        } else if(c == '\'' || c == '"') {
            processString(c, fileContent, result);
        } else {
            processOperator(c, fileContent, result);
            if(c == ';')
            {
                break;
            }
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

