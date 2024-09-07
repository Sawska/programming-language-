#include "parser.h"

ASTNodePtr Parser::parse() {
    ASTNodePtr root = parseLogicalExpression();

    std::cout << "Starting of parsing" << std::endl;

    // Ensure that all tokens have been consumed
    if (index < tokens.size()) {
        throw std::runtime_error("Unexpected tokens after parsing");
    }
    std::cout << "End of parsing" << std::endl;
    return root;
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
            index++; // Skip newline and continue
            left = parseLogicalExpression(); // Re-evaluate to handle possible new statements
        } else {
            break;
        }
    }

    return left;
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
            index++; // Skip newline and continue
            left = parseTerm(); // Re-evaluate term after newline
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
            index++; // Skip newline and continue
            left = parseFactor(); // Re-evaluate factor after newline
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
    }
    
    else if (token.concept == TOKEN::TOKEN_CONCEPTS::STRING) {
        index++;
        return AST::makeStringNode(token.string);
    }
    
    else if (token.concept == TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        index++;
        auto expr = parseExpression();

        if (index >= tokens.size() || tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS) {
            throw std::runtime_error("Mismatched parentheses");
        }
        index++;
        return expr;
    }
    
    else if (token.op == TOKEN::OPERATORS::NOT_OPERATOR ||
             token.op == TOKEN::OPERATORS::INCREMENT_OPERATOR ||
             token.op == TOKEN::OPERATORS::DECREMENT_OPERATOR ||
             token.op == TOKEN::OPERATORS::BIT_NOT_OPERATOR) {
        index++;
        auto operand = parseFactor();
        if (!operand) throw std::runtime_error("Invalid operand for unary operation");
        return AST::makeUnaryOperationNode(token.op, std::move(operand));
    } 
    
    else if (token.concept == TOKEN::TOKEN_CONCEPTS::VARIABLE) {
        index++;
        return parseVariableOrAssignment();
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME) {
        return handleVariableReference();
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

    const TOKEN& token = tokens[index];
    std::string varName = token.variableName;
    index++; // Move past the variable name

    // Check if this is an assignment
    if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::EQUALS_OPERATOR) {
        index++; // Move past the equals operator
        auto expr = parseExpression(); // Parse the right-hand side of the assignment

        // Handle optional newline after assignment
        if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
    }
        table.setVariableValue(varName, std::move(expr)); // Store the value in the symbol table

        return AST::makeEmptyNode(); // Return an empty node for the assignment
    }

    // If it's not an assignment, treat it as a reference to the variable
    return handleVariableReference();
}

ASTNodePtr Parser::handleVariableReference() {
    if (index >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens");
    }
    table.listVariables();
    const TOKEN& token = tokens[index];
    if (token.concept != TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME) {
        throw std::runtime_error("Expected variable name");
    }

    std::string varName = token.variableName;
    index++; 

    
    ASTNodePtr variableNode = table.getVariableValue(varName);
    if (!variableNode) {
        std::cerr << "Undefined variable: " << varName << std::endl;
        throw std::runtime_error("Undefined variable: " + varName);
    }

    return variableNode; // 
}
