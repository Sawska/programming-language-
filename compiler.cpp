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
    auto result = evaluateAST(root);

    if (std::holds_alternative<double>(result)) {
        std::cout << std::get<double>(result) << std::endl;
    } else if (std::holds_alternative<std::string>(result)) {
        std::cout << std::get<std::string>(result) << std::endl;
    }
}





std::variant<double, std::string> Compiler::evaluateAST(const ASTNodePtr& node) {
    if (!node) throw std::runtime_error("Null AST node");

    switch (node->type) {
        case AST::Type::Number: {
            auto nd = dynamic_cast<NumberNode*>(node.get());
            return nd->value;
        }
            

        case AST::Type::String: {
            auto nd = dynamic_cast<StringNode*>(node.get());
            return nd->stringValue;
        }
        

        case AST::Type::Empty:
             return {};
        
        case AST::Type::Block: {
    auto blockNode = dynamic_cast<BlockNode*>(node.get());
    if (!blockNode) throw std::runtime_error("Invalid block node");
    
    symbolTableStack.push(std::move(blockNode->symbol_table));

    for (const ASTNodePtr& statement : blockNode->statements) {
        if (statement->type == AST::Type::Empty) {
            continue;
        }
        
        auto result = evaluateAST(statement);


        if (statement->type == AST::Type::Return) {
            symbolTableStack.pop(); 
            return result;  
        }
    }

    symbolTableStack.pop();
    return {};  
}


    case AST::Type::Return: {
        auto returnNode = dynamic_cast<ReturnNode*>(node.get());

        if (!returnNode) throw std::runtime_error("Invalid return node");
        return evaluateAST(returnNode->value);
    }



        case AST::Type::BinaryOperation: {
           
            if (node->op == TOKEN::OPERATORS::SEQUENCE_OPERATOR) {
                evaluateAST(node->left);
                return evaluateAST(node->right);
            }

            auto leftValue = evaluateAST(node->left);
            auto rightValue = evaluateAST(node->right);

            if (node->op == TOKEN::OPERATORS::PLUS_OPERATOR) {
                if (std::holds_alternative<std::string>(leftValue) || std::holds_alternative<std::string>(rightValue)) {
                    auto convertToString = [](const auto& value) -> std::string {
                        if (std::holds_alternative<std::string>(value)) {
                            return std::get<std::string>(value);
                        } else {
                            double doubleValue = std::get<double>(value);
                            if (std::fabs(doubleValue - std::round(doubleValue)) < std::numeric_limits<double>::epsilon()) {
                                return std::to_string(static_cast<int>(doubleValue));
                            } else {
                                return std::to_string(doubleValue);
                            }
                        }
                    };
                    std::string leftStr = convertToString(leftValue);
                    std::string rightStr = convertToString(rightValue);
                    return leftStr + rightStr;
                }
            }

            double leftNum = std::get<double>(leftValue);
            double rightNum = std::get<double>(rightValue);

            switch (node->op) {
                case TOKEN::OPERATORS::PLUS_OPERATOR:
                    return leftNum + rightNum;
                case TOKEN::OPERATORS::MINUS_OPERATOR:
                    return leftNum - rightNum;
                case TOKEN::OPERATORS::MULTIPLY_OPERATOR:
                    return leftNum * rightNum;
                case TOKEN::OPERATORS::DIVIDE_OPERATOR:
                    if (rightNum == 0) throw std::runtime_error("Division by zero");
                    return leftNum / rightNum;
                case TOKEN::OPERATORS::AND_OPERATOR:
                    return static_cast<double>(static_cast<int>(leftNum) != 0 && static_cast<int>(rightNum) != 0);
                case TOKEN::OPERATORS::OR_OPERATOR:
                    return static_cast<double>(static_cast<int>(leftNum) != 0 || static_cast<int>(rightNum) != 0);
                case TOKEN::OPERATORS::GREATER_THAN_OPERATOR:
                    return static_cast<double>(leftNum > rightNum);
                case TOKEN::OPERATORS::LESS_THAN_OPERATOR:
                    return static_cast<double>(leftNum < rightNum);
                case TOKEN::OPERATORS::COMPARE_OPERATOR:
                    return static_cast<double>(leftNum == rightNum);
                case TOKEN::OPERATORS::NOT_EQUALS_OPERATOR:
                    return static_cast<double>(leftNum != rightNum);
                case TOKEN::OPERATORS::LESS_THAN_EQUAL_OPERATOR:
                    return static_cast<double>(leftNum <= rightNum);
                case TOKEN::OPERATORS::GREATER_THAN_EQUAL_OPERATOR:
                    return static_cast<double>(leftNum >= rightNum);
                case TOKEN::OPERATORS::RIGHT_SHIFT_OPERATOR:
                    return static_cast<double>(static_cast<int>(leftNum) >> static_cast<int>(rightNum));
                case TOKEN::OPERATORS::LEFT_SHIFT_OPERATOR:
                    return static_cast<double>(static_cast<int>(leftNum) << static_cast<int>(rightNum));
                case TOKEN::OPERATORS::BIT_AND_OPERATOR:
                    return static_cast<double>(static_cast<int>(leftNum) & static_cast<int>(rightNum));
                case TOKEN::OPERATORS::BIT_OR_OPERATOR:
                    return static_cast<double>(static_cast<int>(leftNum) | static_cast<int>(rightNum));
                default:
                    throw std::runtime_error("Unsupported binary operator");
            }
        }

        case AST::Type::IF: {
    auto ifNode = dynamic_cast<IfNode*>(node.get());
    
    auto conditionValue = evaluateAST(ifNode->expression);
    
    if (std::holds_alternative<double>(conditionValue)) {
        
        if (std::get<double>(conditionValue)) {
            
            return evaluateAST(ifNode->ifBlock);
        } else {
            
            if (ifNode->elseBlock) {
                return evaluateAST(ifNode->elseBlock);
            }

            return {};
        }
    } else {
        throw std::runtime_error("Condition in if statement did not evaluate to a boolean.");
    }
}



        case AST::Type::UnaryOperation: {
            auto operandResult = evaluateAST(node->left);

            if (std::holds_alternative<double>(operandResult)) {
                double operandValue = std::get<double>(operandResult);
                switch (node->op) {
                    case TOKEN::OPERATORS::NOT_OPERATOR:
                        return static_cast<double>(operandValue == 0);
                    case TOKEN::OPERATORS::BIT_NOT_OPERATOR:
                        return static_cast<double>(~static_cast<int>(operandValue));
                    case TOKEN::OPERATORS::INCREMENT_OPERATOR:
                        return ++operandValue;
                    case TOKEN::OPERATORS::DECREMENT_OPERATOR:
                        return --operandValue;
                    default:
                        throw std::runtime_error("Unsupported unary operator");
                }
            }
            throw std::runtime_error("Unary operations can only be performed on numbers");
        }

        case AST::Type::WHILE: {
            auto whileNode = dynamic_cast<WhileNode*>(node.get());
            auto conditionValue = evaluateAST(whileNode->expression);


            if (std::holds_alternative<double>(conditionValue)) {
        
        if (std::get<double>(conditionValue)) {
            
            return evaluateAST(whileNode->WhileBlock);
        } else {
            return {};
        }
    } else {
        throw std::runtime_error("Condition in while statement did not evaluate to a boolean.");
    }
        }



        

        case AST::Type::ParenthesizedExpression:
            return evaluateAST(node->left);

        case AST::Type::Variable: {
    auto varNode = dynamic_cast<VariableNode*>(node.get());
    if (!varNode) throw std::runtime_error("Invalid variable node");
    return evaluateAST(varNode->value);
}


        default:
            throw std::runtime_error("Unknown AST node type");
    }
}
