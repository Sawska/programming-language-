#include "compiler.h"

void Compiler::run() {
    std::cout << "starting of compiler work" << std::endl;
    if(!ISREPL)
    {
        REPL();
        std::cout << "end of REPL" << std::endl;
    } else {
        
    }
}

void Compiler::REPL() {
    std::cout << evaluateAST(root) << std::endl;

}

#include <stdexcept>

double Compiler::evaluateAST(const ASTNodePtr& node) {
    if (!node) throw std::runtime_error("Null AST node");

    switch (node->type) {
        case AST::Type::Number:
            return node->value;

        case AST::Type::BinaryOperation: {
            double leftValue = evaluateAST(node->left);
            double rightValue = evaluateAST(node->right);

            switch (node->op) {
                case TOKEN::OPERATORS::PLUS_OPERATOR:
                    return leftValue + rightValue;
                case TOKEN::OPERATORS::MINUS_OPERATOR:
                    return leftValue - rightValue;
                case TOKEN::OPERATORS::MULTIPLY_OPERATOR:
                    return leftValue * rightValue;
                case TOKEN::OPERATORS::DIVIDE_OPERATOR:
                    return leftValue / rightValue;
                case TOKEN::OPERATORS::AND_OPERATOR:
                    
                    return static_cast<double>(leftValue != 0 && rightValue != 0);
                case TOKEN::OPERATORS::OR_OPERATOR:
                    return static_cast<double>(leftValue != 0 || rightValue != 0);
                case TOKEN::OPERATORS::GREATER_THAN_OPERATOR:
                    return static_cast<double>(leftValue > rightValue);
                case TOKEN::OPERATORS::LESS_THAN_OPERATOR:
                    return static_cast<double>(leftValue < rightValue);
                case TOKEN::OPERATORS::EQUALS_OPERATOR:
                    return static_cast<double>(leftValue == rightValue);
                case TOKEN::OPERATORS::NOT_EQUALS_OPERATOR:
                    return static_cast<double>(leftValue != rightValue);
                case TOKEN::OPERATORS::LESS_THAN_EQUAL_OPERATOR:
                    return static_cast<double>(leftValue <= rightValue);
                case TOKEN::OPERATORS::GREATER_THAN_EQUAL_OPERATOR:
                std::cout << "here" << std::endl;
                    return static_cast<double>(leftValue >= rightValue);
                default:
                    throw std::runtime_error("Unsupported binary operator");
            }
        }

        case AST::Type::UnaryOperation: {
            double operandValue = evaluateAST(node->left);
            switch (node->op) {
                case TOKEN::OPERATORS::NOT_OPERATOR:
                    return static_cast<double>(operandValue == 0);
                case TOKEN::OPERATORS::BIT_NOT_OPERATOR:
                        return ~static_cast<int>(operandValue);
                case TOKEN::OPERATORS::INCREMENT_OPERATOR:
                        return operandValue++;
                case TOKEN::OPERATORS::DECREMENT_OPERATOR: 
                        return operandValue--;
                default:
                    throw std::runtime_error("Unsupported unary operator");
            }
        }

        case AST::Type::ParenthesizedExpression:
            return evaluateAST(node->left);

        default:
            throw std::runtime_error("Unknown AST node type");
    }
}
