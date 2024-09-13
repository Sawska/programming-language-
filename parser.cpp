#include "parser.h"

ASTNodePtr Parser::parse() {
    ASTNodePtr root = nullptr;
    symbolTableStack.push(std::make_unique<SymbolTable>());
    std::cout << "Starting of parsing" << std::endl;

    while (index < tokens.size()) {
        ASTNodePtr line = parseLogicalExpression();

        if (!root) {
            root = std::move(line);
        } else {
            root = AST::makeSequenceNode(std::move(root), std::move(line));
        }

        if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
        }
    }

    if (index < tokens.size()) {
        throw std::runtime_error("Unexpected tokens after parsing");
    }

    std::cout << "End of parsing" << std::endl;
    return root;
}

ASTNodePtr Parser::parseExpression() {
    auto left = parseTerm();

    while (index < tokens.size()) {
        const TOKEN& token = tokens[index];
        if (token.op == TOKEN::OPERATORS::PLUS_OPERATOR ||
            token.op == TOKEN::OPERATORS::MINUS_OPERATOR ||
            token.op == TOKEN::OPERATORS::GREATER_THAN_OPERATOR ||
            token.op == TOKEN::OPERATORS::GREATER_THAN_EQUAL_OPERATOR ||
            token.op == TOKEN::OPERATORS::LESS_THAN_EQUAL_OPERATOR ||
            token.op == TOKEN::OPERATORS::LESS_THAN_OPERATOR ||
            token.op == TOKEN::OPERATORS::COMPARE_OPERATOR ||
            token.op == TOKEN::OPERATORS::NOT_EQUALS_OPERATOR ||
            token.op == TOKEN::OPERATORS::EQUAL_PLUS_OPERATOR ||
            token.op == TOKEN::OPERATORS::EQUAL_MINUS_OPERATOR) {
            index++;
            auto right = parseTerm();
            left = AST::makeBinaryOperationNode(token.op, std::move(left), std::move(right));
        } else if (token.op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
            break;
        } else {
            break;
        }
    }

    return left;
}

ASTNodePtr Parser::parseLogicalExpression() {
    auto left = parseExpression();

    while (index < tokens.size()) {
        const TOKEN& token = tokens[index];
        if (token.op == TOKEN::OPERATORS::AND_OPERATOR ||
            token.op == TOKEN::OPERATORS::OR_OPERATOR ||
            token.op == TOKEN::OPERATORS::LEFT_SHIFT_OPERATOR ||
            token.op == TOKEN::OPERATORS::RIGHT_SHIFT_OPERATOR ||
            token.op == TOKEN::OPERATORS::LEFT_SHIFT_EQUAL_OPERATOR ||
            token.op == TOKEN::OPERATORS::RIGHT_SHIFT_EQUAL_OPERATOR ||
            token.op == TOKEN::OPERATORS::NOT_EQUALS_OPERATOR ||
            token.op == TOKEN::OPERATORS::BIT_AND_OPERATOR ||
            token.op == TOKEN::OPERATORS::BIT_OR_OPERATOR) {
            index++;
            auto right = parseExpression();
            left = AST::makeBinaryOperationNode(token.op, std::move(left), std::move(right));
        } else if (token.op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
            break;
        } else {
            break;
        }
    }

    return left;
}

ASTNodePtr Parser::parseTerm() {
    auto left = parseFactor();

    while (index < tokens.size()) {
        const TOKEN& token = tokens[index];
        if (token.op == TOKEN::OPERATORS::MULTIPLY_OPERATOR ||
            token.op == TOKEN::OPERATORS::DIVIDE_OPERATOR ||
            token.op == TOKEN::OPERATORS::EQUAL_DIVIDE_OPERATOR ||
            token.op == TOKEN::OPERATORS::EQUAL_MULTIPLY_OPERATOR) {
            index++;
            auto right = parseFactor();
            left = AST::makeBinaryOperationNode(token.op, std::move(left), std::move(right));
        } else if (token.op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
            break;
        } else {
            break;
        }
    }

    return left;
}

ASTNodePtr Parser::parseFactor() {
    if (index >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens");
    }

    const TOKEN& token = tokens[index];

    if (token.concept == TOKEN::TOKEN_CONCEPTS::NUMBER) {
        index++;
        return AST::makeNumberNode(token.number);
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::STRING) {
        index++;
        return AST::makeStringNode(token.string);
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS) {
        index++;
        auto expr = parseExpression();

        if (index >= tokens.size() || tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE_BRACKETS) {
            throw std::runtime_error("Mismatched parentheses");
        }
        index++;
        return expr;
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        return parseBlock();
    } else if (token.op == TOKEN::OPERATORS::NOT_OPERATOR ||
               token.op == TOKEN::OPERATORS::INCREMENT_OPERATOR ||
               token.op == TOKEN::OPERATORS::DECREMENT_OPERATOR ||
               token.op == TOKEN::OPERATORS::BIT_NOT_OPERATOR) {
        index++;
        auto operand = parseFactor();
        if (!operand) throw std::runtime_error("Invalid operand for unary operation");
        return AST::makeUnaryOperationNode(token.op, std::move(operand));
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::VARIABLE) {
        index++;
        return parseVariableOrAssignment();
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME) {
        return handleVariableReference();
    }  else if(token.concept == TOKEN::TOKEN_CONCEPTS::IF) {
        return parseIf();
    }
    else {
        std::cerr << "Unexpected token: " << token.op << " at index: " << index << std::endl;
        throw std::runtime_error("Unexpected token: " + std::to_string(token.op));
    }
}


ASTNodePtr Parser::parseVariableOrAssignment() {
    if (index >= tokens.size() || tokens[index].concept != TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME) {
        std::cerr << "Expected variable name but got a different token" << std::endl;
        throw std::runtime_error("Expected variable name");
    }


    SymbolTable& currentTable = *symbolTableStack.top();

    const TOKEN& token = tokens[index];
    std::string varName = token.variableName;
    index++;

    if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::EQUALS_OPERATOR) {
        index++;
        auto expr = parseExpression();

        if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
        }
        currentTable.setVariableValue(varName, std::move(expr));
        return AST::makeEmptyNode();
    }

    return handleVariableReference();
}


ASTNodePtr Parser::parseBlock() {
    index++;
    auto block_node = std::make_unique<BlockNode>();

    
    symbolTableStack.push(std::make_unique<SymbolTable>());

    
    while (index < tokens.size() && tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS) {
        auto statement = parseStatement();
        if (statement) {
            block_node->addStatement(std::move(statement));
        }
    }

    
    if (index < tokens.size() && tokens[index].concept == TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS) {
        index++;
    } else {
        throw std::runtime_error("Mismatched or missing closing bracket");
    }


    block_node->symbol_table = std::move(symbolTableStack.top());


    symbolTableStack.pop();

    return block_node;
}




ASTNodePtr Parser::handleVariableReference() {
    if (index >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens");
    }

    SymbolTable& currentTable = *symbolTableStack.top();
    const TOKEN& token = tokens[index];

    if (token.concept != TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME) {
        throw std::runtime_error("Expected variable name");
    }

    std::string varName = token.variableName;
    index++;


    if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::EQUALS_OPERATOR) {
        index++;
        auto expr = parseExpression();

        if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
        }
        currentTable.setVariableValue(varName, std::move(expr));
        return AST::makeEmptyNode();        
    }


    ASTNodePtr variableValueNode = currentTable.getVariableValue(varName);

    if (!variableValueNode) {
        std::cerr << "Undefined variable: " << varName << std::endl;
        throw std::runtime_error("Undefined variable: " + varName);
    }


    return std::make_unique<VariableNode>(varName, std::move(variableValueNode));
}



ASTNodePtr Parser::parseStatement() {
    if (index >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens");
    }

    const TOKEN& token = tokens[index];

    if (token.concept == TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME) {

        return handleVariableReference();
    }  else if(token.concept == TOKEN::TOKEN_CONCEPTS::VARIABLE)
    {
        index++;
        return parseVariableOrAssignment();
    }
    else if (token.concept == TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS) {
        
        return parseExpression();
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        
        return parseBlock();
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::IF) {

        return parseIf();
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::WHILE) {

        return parseWhile();
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::RETURN) {

        return parseReturn();
    } else {
        std::cerr << "Unexpected token: " << token.variableName << " at index: " << index << std::endl;
        throw std::runtime_error("Unexpected token: " + std::to_string(token.concept));
    }
}

ASTNodePtr Parser::parseIf() {
    index++;  
    auto node = std::make_unique<IfNode>(nullptr, nullptr);  

    
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS) {
        std::cerr << "Error: 'if' statement needs an opening '('." << std::endl;
        throw std::runtime_error("Expected '(' after 'if'");
    }

    index++;  


    node->expression = parseLogicalExpression();  

    
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE_BRACKETS) {
        throw std::runtime_error("Error: Mismatched or missing ')'.");
    }

    index++; 

    
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        throw std::runtime_error("Error: 'if' statement needs an opening '{'.");
    }


    
    node->ifBlock = parseBlock();  

    
    if (tokens[index].concept == TOKEN::TOKEN_CONCEPTS::ELSE) {
        index++; 

        if (tokens[index].concept == TOKEN::TOKEN_CONCEPTS::IF) {
            
            node->elseBlock = parseIf();  
        } else if (tokens[index].concept == TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {

            index++; 
            node->elseBlock = parseBlock();  
        } else {
            throw std::runtime_error("Error: Expected '{' after 'else'.");
        }
    }

    return node;
}



ASTNodePtr Parser::parseWhile()
{
    index++;
    auto node = std::make_unique<WhileNode>(nullptr, nullptr);      

    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS) {
        std::cerr << "Error: 'while' statement needs an opening '('." << std::endl;
        throw std::runtime_error("Expected '(' after 'while'");
    }
    index++;  
    node->expression = parseLogicalExpression();  
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE_BRACKETS) {
        throw std::runtime_error("Error: Mismatched or missing ')'.");
    }
    index++; 
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        throw std::runtime_error("Error: 'while' statement needs an opening '{'.");
    }
    node->WhileBlock = parseBlock();  

    return node;
}

ASTNodePtr Parser::parseReturn()
{
    index++;
        auto expr = parseExpression();

        if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
        }
        return std::make_unique<ReturnNode>(std::move(expr));
}

