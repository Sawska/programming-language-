#include "compiler.h"

template <typename T>
void printIfType(const ASTResult& result) {
    if (std::holds_alternative<T>(result)) {
        std::cout << std::get<T>(result) << std::endl;
    }
}







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
    printIfType<double>(result);
    printIfType<std::string>(result);
    printIfType<bool>(result);
     if (std::holds_alternative<ReturnType>(result)) {
         auto res = evaluateAST(std::get<ReturnType>(result).returnValue);
         if(std::holds_alternative<double>(res)) {
            std::cout << std::get<double>(res) << std::endl;
         }
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
            auto nd = static_cast<NumberNode*>(node.get());
            return nd->value;
        }
            

        case AST::Type::String: {
            auto nd = static_cast<StringNode*>(node.get());
            return nd->stringValue;
        }
        

        case AST::Type::Empty:
             return {};
        
        case AST::Type::Block: {
    auto blockNode = static_cast<BlockNode*>(node.get());
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

    case AST::Type::ObjectAccess: {
        auto objectAccessNode = static_cast<ObjectAccessNode*>(node.get());
        auto objectNode = static_cast<ObjectNode*>(objectAccessNode->baseObject.get());
        if(objectAccessNode->memberName != "") {
            return evaluateAST(objectNode->attributes->getVariableValue(objectAccessNode->memberName));
        } else if(objectAccessNode->methodName != "") {
            return evaluateAST(objectNode->methods->getVariableValue(objectAccessNode->methodName));
        }
    }


    case AST::Type::ClassAccess: {
        auto classAccessNode = static_cast<ClassAccessNode*>(node.get());
        auto classNode = static_cast<ClassNode*>(classAccessNode->baseClass.get());
        if(classAccessNode->memberName != "") {
            return evaluateAST(classNode->getField(classAccessNode->memberName));
        } else if(classAccessNode->methodName != "") {
            return evaluateAST(classNode->getMethod(classAccessNode->methodName));
        }
    }


    case AST::Type::Return: {
    if (auto returnNode = static_cast<ReturnNode*>(node.get())) {
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

    case AST::Type::Array: {
    auto arrayNode = static_cast<ArrayNode*>(node.get());
    if (!arrayNode) {
        throw std::runtime_error("Invalid node type for array.");
    }


    return std::move(arrayNode->array);
    
}


    
        case AST::Type::Access: {
    auto AccessNode = static_cast<ArrayAccessNode*>(node.get());
    if (!AccessNode) {
        throw std::runtime_error("Invalid node type for array access.");
    }

    auto result = evaluateAST(AccessNode->indexExpression);

    if (std::holds_alternative<double>(result)) {
        double index = std::get<double>(result);
        std::string name = AccessNode->variableName;


        auto varPtr = findVariableInSymbolTableStack(name, *symbolTableStack.top());
        

        


        
        ArrayNode* arrNode = static_cast<ArrayNode*>(varPtr.get());

        if (!arrNode) {
            throw std::runtime_error("Variable is not an array.");
        }


        if (index < 0 || index >= arrNode->array.size()) {
            throw std::out_of_range("Array index out of bounds.");
        }


        auto element = evaluateAST(arrNode->array[index]);
        return element;

    } else {
        throw std::runtime_error("Index must be a number.");
    }
}


        case AST::Type::Function: {
    auto functionNode = static_cast<FunctionNode*>(node.get());
    

    auto table = std::make_unique<SymbolTable>();
    
    for (int i = 0; i < functionNode->argument_list.size(); i++) {
        auto val = static_cast<VariableNode*>(functionNode->argument_list[i].get());
        if (val) {
            
            table->setVariableValue(val->name, std::move(functionNode->argument_list[i])); 
        } else {
            throw std::runtime_error("Invalid variable node");
        }
    }
    
    if (!functionNode) {
        throw std::runtime_error("Invalid function node");
    }


    symbolTableStack.push(std::move(table));

    auto res= evaluateAST(functionNode->block);
    symbolTableStack.pop();
    return res;
}



        case AST::Type::BinaryOperation: {
           auto binaryfNode = static_cast<BinaryOperationNode*>(node.get());
           
            if (binaryfNode->op == TOKEN::OPERATORS::SEQUENCE_OPERATOR) {
                evaluateAST(binaryfNode->left);
                return evaluateAST(binaryfNode->right);
            }


            auto leftValue = evaluateAST(binaryfNode->left);
            auto leftVar = static_cast<VariableNode*>(binaryfNode->left.get());
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
                case TOKEN::OPERATORS::EQUAL_DIVIDE_OPERATOR: {
                    if (rightNum == 0) throw std::runtime_error("Division by zero");
                    auto res =  leftNum / rightNum;
                    if(leftVar) {
                        update_variable(leftVar->name,AST::makeNumberNode(res));
                    }
                    return res;
                }
                case TOKEN::OPERATORS::EQUAL_MINUS_OPERATOR: {
                    auto res =  leftNum - rightNum;
                        if(leftVar) {
                            update_variable(leftVar->name,AST::makeNumberNode(res));
                        }
                    return res;
                }

                case TOKEN::OPERATORS::EQUAL_MULTIPLY_OPERATOR: {
                    auto res =  leftNum * rightNum;
                        if(leftVar) {
                            update_variable(leftVar->name,AST::makeNumberNode(res));
                        }
                    return res;
                }
                case TOKEN::OPERATORS::EQUAL_PLUS_OPERATOR: {
                    auto res =  leftNum + rightNum;
                        if(leftVar) {
                            update_variable(leftVar->name,AST::makeNumberNode(res));
                        }
                    return res;
                }
                default:
                    throw std::runtime_error("Unsupported binary operator");
            }
        }

        case AST::Type::IF: {
    auto ifNode = static_cast<IfNode*>(node.get());
    
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

    case AST::Type::Bool: {
        auto boolNode = static_cast<BoolNode*>(node.get());
        return boolNode->value;
    }

    case AST::Type::Class: {
    auto classNode = static_cast<ClassNode*>(node.get());    
    return std::unique_ptr<AST>(classNode); 
    }

    case AST::Type::Object: {
    auto objectNode = static_cast<ObjectNode*>(node.get());
    if (objectNode) {


    ASTNodePtr astNodePtr = std::unique_ptr<ObjectNode>(objectNode); 

    return std::make_unique<ObjectNode>(std::move(astNodePtr));

    } else {
        throw std::runtime_error("Invalid object node");
    }
}


    





        case AST::Type::UnaryOperation: {
    auto unaryNode = static_cast<UnaryOperationNode*>(node.get());
    auto operandResult = evaluateAST(unaryNode->operand);

    
    auto var = static_cast<VariableNode*>(unaryNode->operand.get());

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
                } 
                else {
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
    if (auto whileNode = static_cast<WhileNode*>(node.get())) {
        loopStack.push(true);
        while (true) {
            
            auto conditionValue = evaluateAST(whileNode->expression);
            if (!std::holds_alternative<double>(conditionValue) || !std::get<double>(conditionValue)) {
                break;  
            }
            
            
            auto result = evaluateAST(whileNode->WhileBlock);
            
            
            if (std::holds_alternative<BreakType>(result)) {
                break;
            }
            if (std::holds_alternative<ReturnType>(result)) {
                loopStack.pop();
                ReturnType retr = std::move(std::get<ReturnType>(result));
                return evaluateAST(retr.returnValue);
            }
            if (std::holds_alternative<ContinueType>(result)) {
                continue;
            }
        }
        
        return VoidType{};
    }
}


case AST::Type::FOR: {
    if (auto forNode = static_cast<ForNode*>(node.get())) {
        loopStack.push(true);  

        
        evaluateAST(forNode->initialization);

        
        while (true) {
            auto conditionValue = evaluateAST(forNode->expression);
            if (!std::holds_alternative<double>(conditionValue) || !std::get<double>(conditionValue)) {
                break; 
            }

            
            auto result = evaluateAST(forNode->forBlock);

            
            if (std::holds_alternative<BreakType>(result)) {
                loopStack.pop();
                break;
            }
            if (std::holds_alternative<ReturnType>(result)) {
                loopStack.pop();
                ReturnType retr = std::move(std::get<ReturnType>(result));
                return evaluateAST(retr.returnValue);
            }
            if (std::holds_alternative<ContinueType>(result)) {
            
                evaluateAST(forNode->increment);  
                continue;
            }

            
            std::cout << std::get<double>(evaluateAST(forNode->increment)) << std::endl;



            std::cout << "" << std::endl;
        }

        return VoidType{};  
    }
}




        

        case AST::Type::ParenthesizedExpression:
            return evaluateAST(node->left);

        case AST::Type::Variable: {
    auto varNode = static_cast<VariableNode*>(node.get());
    if (!varNode) throw std::runtime_error("Invalid variable node");
    return evaluateAST(find_variable_value(varNode->name));
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



ASTNodePtr Compiler::findVariableInSymbolTableStack(const std::string& varName, SymbolTable& currentTable) {

    ASTNodePtr variableValueNode = currentTable.getVariableValue(varName);

    if (variableValueNode) {
        return variableValueNode;
    }


    std::stack<std::unique_ptr<SymbolTable>> tempStack;
    bool found = false;


    while (!symbolTableStack.empty()) {
        std::unique_ptr<SymbolTable> tempTable = std::move(symbolTableStack.top());
        symbolTableStack.pop();
        tempStack.push(std::move(tempTable));
        variableValueNode = tempStack.top()->getVariableValue(varName);
        
        if (variableValueNode) {
            found = true;
            break;
        }
    }

    
    while (!tempStack.empty()) {
        symbolTableStack.push(std::move(tempStack.top()));
        tempStack.pop();
    }

    if (!found) {
        std::cerr << "Undefined variable: " << varName << std::endl;
        throw std::runtime_error("Undefined variable: " + varName);
    }

    return variableValueNode;
}


ASTNodePtr Compiler::find_variable_value(const std::string& varName) {
    std::stack<std::unique_ptr<SymbolTable>> tempStack;
    ASTNodePtr variableValueNode;

    while (!symbolTableStack.empty()) {
        SymbolTable* currentTable = symbolTableStack.top().get();
        

        variableValueNode = currentTable->getVariableValue(varName);

        if (variableValueNode) {
            
            break;
        }

        
        tempStack.push(std::move(symbolTableStack.top()));
        symbolTableStack.pop();
    }

    
    while (!tempStack.empty()) {
        symbolTableStack.push(std::move(tempStack.top()));
        tempStack.pop();
    }

    return variableValueNode;


}
