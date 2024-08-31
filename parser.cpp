#include "parser.h"

ASTNodePtr Parser::parse() {

    return parseLogicalExpression();
}

ASTNodePtr Parser::parseLogicalExpression() {
    auto left = parseExpression();

    while (index < tokens.size()) {
        const TOKEN& token = tokens[index];
        if (token.op == TOKEN::OPERATORS::AND_OPERATOR || token.op == TOKEN::OPERATORS::OR_OPERATOR) {
            index++;
            auto right = parseExpression();
            left = AST::makeBinaryOperationNode(token.op, std::move(left), std::move(right));
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
        if (token.op == TOKEN::OPERATORS::PLUS_OPERATOR || token.op == TOKEN::OPERATORS::MINUS_OPERATOR || 
            token.op == TOKEN::OPERATORS::BIGGER_OPERATOR || token.op == TOKEN::OPERATORS::SMALLER_OPERATOR || 
            token.op == TOKEN::OPERATORS::EQUALS_OPERATOR) {
            index++;
            auto right = parseTerm();
            left = AST::makeBinaryOperationNode(token.op, std::move(left), std::move(right));
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
        if (token.op == TOKEN::OPERATORS::MULTIPLY_OPERATOR || token.op == TOKEN::OPERATORS::DIVIDE_OPERATOR) {
            index++;
            auto right = parseFactor();
            left = AST::makeBinaryOperationNode(token.op, std::move(left), std::move(right));
        } else {
            break;
        }
    }

    return left;
}

ASTNodePtr Parser::parseFactor() {
    const TOKEN& token = tokens[index];

    if (token.concept == TOKEN::TOKEN_CONCEPTS::NUMBER) {
        index++;
        return AST::makeNumberNode(token.number); 
    } else if (token.concept == TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        index++;
        auto expr = parseExpression();

        if (index >= tokens.size() || tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS) {
            throw std::runtime_error("Mismatched parentheses");
        }
        index++;
        return expr;
    } else if (token.op == TOKEN::OPERATORS::NOT_OPERATOR) {
        index++;
        auto operand = parseFactor();
        return AST::makeUnaryOperationNode(token.op, std::move(operand));
    } else {
        throw std::runtime_error("Unexpected token");
    }
}
