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
    } else if(std::holds_alternative<ReturnType>(result))
    {
        ReturnType retr =  std::move(std::get<ReturnType>(result));
        
    } else if(std::holds_alternative<BreakType>(result))
    {

    } else if(std::holds_alternative<ContinueType>(result)) {
        
    } else if(std::holds_alternative<VoidType>(result)) {

    }
}





ASTResult Compiler::evaluateAST(const ASTNodePtr& node) {
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

        if (statement->type == AST::Type::Break) {
            loopStack.pop();
            return BreakType{}; 
        }
        
        if (statement->type == AST::Type::Continue) {
            
            if (loopStack.top()) {
                return ContinueType{};
            } else {
                throw std::runtime_error("Continue statement outside of a loop.");
            }
        }

        if (statement->type == AST::Type::Return) {
            symbolTableStack.pop(); 
            return result;
        }
    }

    symbolTableStack.pop();
    return VoidType{};
}


    case AST::Type::Return: {
    if (auto returnNode = dynamic_cast<ReturnNode*>(node.get())) {
        return ReturnType(std::move(returnNode->value));
    } else {
        throw std::runtime_error("Invalid return node");
    }
}

    case AST::Type::Break: {
    if (!loopStack.empty() && loopStack.top()) {
        return BreakType{};  
    } else {
        throw std::runtime_error("Break statement outside of a loop.");
    }
}



    case AST::Type::Continue: {
    if (!loopStack.empty() && loopStack.top()) {
        return ContinueType{};  
    } else {
        throw std::runtime_error("Continue statement outside of a loop.");
    }
}



        case AST::Type::BinaryOperation: {
           auto binaryfNode = dynamic_cast<BinaryOperationNode*>(node.get());
            if (binaryfNode->op == TOKEN::OPERATORS::SEQUENCE_OPERATOR) {
                evaluateAST(binaryfNode->left);
                return evaluateAST(binaryfNode->right);
            }

            auto leftValue = evaluateAST(binaryfNode->left);
            auto rightValue = evaluateAST(binaryfNode->right);

            if (binaryfNode->op == TOKEN::OPERATORS::PLUS_OPERATOR) {
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

            switch (binaryfNode->op) {
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

            return VoidType{};
        }
    } else {
        throw std::runtime_error("Condition in if statement did not evaluate to a boolean.");
    }
}



        case AST::Type::UnaryOperation: {
    auto unaryNode = dynamic_cast<UnaryOperationNode*>(node.get());
    auto operandResult = evaluateAST(unaryNode->operand);

    
    auto var = dynamic_cast<VariableNode*>(unaryNode->operand.get());

    if (std::holds_alternative<double>(operandResult)) {
        double operandValue = std::get<double>(operandResult);

        switch (unaryNode->op) {
            case TOKEN::OPERATORS::NOT_OPERATOR:
                return static_cast<double>(operandValue == 0);

            case TOKEN::OPERATORS::BIT_NOT_OPERATOR:
                return static_cast<double>(~static_cast<int>(operandValue));

            case TOKEN::OPERATORS::INCREMENT_OPERATOR: {
                if (var) {
                    
                    ASTNodePtr node = AST::makeNumberNode(++operandValue);
                    update_variable(var->name, std::move(node));  
                    return ++operandValue;  
                } else {
                    return ++operandValue;
                }
            }

            case TOKEN::OPERATORS::DECREMENT_OPERATOR: {
                if (var) {

                    ASTNodePtr node = AST::makeNumberNode(--operandValue);
                    update_variable(var->name, std::move(node));  
                    return VoidType{};  
                } else {
                    return --operandValue;
                }
            }

            default:
                throw std::runtime_error("Unsupported unary operator");
        }
    }

    throw std::runtime_error("Unary operations can only be performed on numbers");
}


        case AST::Type::WHILE: {
    if (auto whileNode = dynamic_cast<WhileNode*>(node.get())) {
        loopStack.push(true);
        while (true) {
            
            auto conditionValue = evaluateAST(whileNode->expression);
            if (!std::holds_alternative<double>(conditionValue) || !std::get<double>(conditionValue)) {
                break;  
            }
            
            
            auto result = evaluateAST(whileNode->WhileBlock);
            
            
            if (std::holds_alternative<BreakType>(result)) {
                return VoidType{};          
                break;
            }
            if (std::holds_alternative<ReturnType>(result)) {
                loopStack.pop();
                return result;
            }
            if (std::holds_alternative<ContinueType>(result)) {
                continue;
            }
        }
        loopStack.pop(); 
        return VoidType{};
    }
}


case AST::Type::FOR: {
    if (auto forNode = dynamic_cast<ForNode*>(node.get())) {
        loopStack.push(true);  

        
        evaluateAST(forNode->initialization);

        
        while (true) {
            auto conditionValue = evaluateAST(forNode->expression);
            if (!std::holds_alternative<double>(conditionValue) || !std::get<double>(conditionValue)) {
                break; 
            }

            
            auto result = evaluateAST(forNode->forBlock);

            
            if (std::holds_alternative<BreakType>(result)) {
                return VoidType{};          
                break;
            }
            if (std::holds_alternative<ReturnType>(result)) {
                loopStack.pop();
                return result;
            }
            if (std::holds_alternative<ContinueType>(result)) {
            
                evaluateAST(forNode->increment);  
                continue;
            }

            
            std::cout << std::get<double>(evaluateAST(forNode->increment)) << std::endl;



            std::cout << "" << std::endl;
        }

        loopStack.pop();
        return VoidType{};  
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


void Compiler::update_variable(const std::string& varName, ASTNodePtr node) {
    
    std::stack<std::unique_ptr<SymbolTable>> tempStack;
    bool found = false;

    
    while (!symbolTableStack.empty()) {
        
        SymbolTable* currentTable = symbolTableStack.top().get();
        
        
        ASTNodePtr variableValueNode = currentTable->getVariableValue(varName);

        if (variableValueNode) {
            
            currentTable->setVariableValue(varName, std::move(node));
            found = true;
            break;
        }


        tempStack.push(std::move(symbolTableStack.top()));
        symbolTableStack.pop();
    }


    while (!tempStack.empty()) {
        symbolTableStack.push(std::move(tempStack.top()));
        tempStack.pop();
    }


    if (!found) {
        std::cerr << "Undefined variable: " << varName << std::endl;
        throw std::runtime_error("Undefined variable: " + varName);
    }
}
