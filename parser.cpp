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
        switch (token.op) {
            case TOKEN::OPERATORS::PLUS_OPERATOR:
            case TOKEN::OPERATORS::MINUS_OPERATOR:
            case TOKEN::OPERATORS::GREATER_THAN_OPERATOR:
            case TOKEN::OPERATORS::GREATER_THAN_EQUAL_OPERATOR:
            case TOKEN::OPERATORS::LESS_THAN_EQUAL_OPERATOR:
            case TOKEN::OPERATORS::LESS_THAN_OPERATOR:
            case TOKEN::OPERATORS::COMPARE_OPERATOR:
            case TOKEN::OPERATORS::NOT_EQUALS_OPERATOR:
            case TOKEN::OPERATORS::EQUAL_PLUS_OPERATOR:
            case TOKEN::OPERATORS::EQUAL_MINUS_OPERATOR:
                index++;
                left = AST::makeBinaryOperationNode(token.op, parseTerm(), std::move(left));
                break;
            case TOKEN::OPERATORS::NEWLINE_OPERATOR:
                index++;
                return left;
            default:
                return left;
        }
    }

    return left;
}

ASTNodePtr Parser::parseLogicalExpression() {
    auto left = parseExpression();

    while (index < tokens.size()) {
        const TOKEN& token = tokens[index];
        switch (token.op) {
            case TOKEN::OPERATORS::AND_OPERATOR:
            case TOKEN::OPERATORS::OR_OPERATOR:
            case TOKEN::OPERATORS::LEFT_SHIFT_OPERATOR:
            case TOKEN::OPERATORS::RIGHT_SHIFT_OPERATOR:
            case TOKEN::OPERATORS::LEFT_SHIFT_EQUAL_OPERATOR:
            case TOKEN::OPERATORS::RIGHT_SHIFT_EQUAL_OPERATOR:
            case TOKEN::OPERATORS::NOT_EQUALS_OPERATOR:
            case TOKEN::OPERATORS::BIT_AND_OPERATOR:
            case TOKEN::OPERATORS::BIT_OR_OPERATOR:
                index++;
                left = AST::makeBinaryOperationNode(token.op, parseExpression(), std::move(left));
                break;
            case TOKEN::OPERATORS::NEWLINE_OPERATOR:
                index++;
                return left;
            default:
                if (token.concept == TOKEN::TOKEN_CONCEPTS::SEMICOLON)
                    return left;
                return left;
        }
    }

    return left;
}

ASTNodePtr Parser::parseTerm() {
    auto left = parseFactor();

    while (index < tokens.size()) {
        const TOKEN& token = tokens[index];
        switch (token.op) {
            case TOKEN::OPERATORS::MULTIPLY_OPERATOR:
            case TOKEN::OPERATORS::DIVIDE_OPERATOR:
            case TOKEN::OPERATORS::EQUAL_DIVIDE_OPERATOR:
            case TOKEN::OPERATORS::EQUAL_MULTIPLY_OPERATOR:
                index++;
                left = AST::makeBinaryOperationNode(token.op, parseFactor(), std::move(left));
                break;
            case TOKEN::OPERATORS::NEWLINE_OPERATOR:
            case TOKEN::TOKEN_CONCEPTS::COMMA:
                index++;
                return left;
            default:
                return left;
        }
    }

    return left;
}

ASTNodePtr Parser::parseFactor() {
    if (index >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens");
    }

    const TOKEN& token = tokens[index];
    switch (token.concept) {
        case TOKEN::TOKEN_CONCEPTS::NUMBER:
            index++;
            return AST::makeNumberNode(token.number);
        case TOKEN::TOKEN_CONCEPTS::STRING:
            index++;
            return AST::makeStringNode(token.string);
        case TOKEN::TOKEN_CONCEPTS::TRUE:
            return std::make_unique<BoolNode>(true);
        case TOKEN::TOKEN_CONCEPTS::FALSE:
            return std::make_unique<BoolNode>(false);
        case TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS:
            index++;
            {
                auto expr = parseExpression();
                if (index >= tokens.size() || tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE_BRACKETS) {
                    throw std::runtime_error("Mismatched parentheses");
                }
                index++;
                return expr;
            }
        case TOKEN::TOKEN_CONCEPTS::OPEN_SQUARE_BRACKETS:
            index++;
            return parseArray();
        case TOKEN::TOKEN_CONCEPTS::NEW:
            index++;
            return parseObjectInstance();
        case TOKEN::TOKEN_CONCEPTS::FOR:
            index++;
            return parseFor();
        case TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS:
            return parseBlock();
        case TOKEN::TOKEN_CONCEPTS::VARIABLE:
            index++;
            return parseVariableOrAssignment();
        case TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME:
            return handleVariableReference();
        case TOKEN::TOKEN_CONCEPTS::WHILE:
            return parseWhile();
        case TOKEN::TOKEN_CONCEPTS::IF:
            return parseIf();
        case TOKEN::TOKEN_CONCEPTS::RETURN:
            return parseReturn();
        case TOKEN::TOKEN_CONCEPTS::FUNCTION:
            return parseFunction();
        case TOKEN::TOKEN_CONCEPTS::FUNCTION_NAME:
            return handleFunctionRefrence();
        case TOKEN::TOKEN_CONCEPTS::CLASS:
            return parseClass();
        default:
            switch (token.op) {
                case TOKEN::OPERATORS::NOT_OPERATOR:
                case TOKEN::OPERATORS::INCREMENT_OPERATOR:
                case TOKEN::OPERATORS::DECREMENT_OPERATOR:
                case TOKEN::OPERATORS::BIT_NOT_OPERATOR:
                    index++;
                    return AST::makeUnaryOperationNode(token.op, parseFactor());
                default:
                    std::cerr << "Unexpected token: " << token.op << " at index: " << index << std::endl;
                    throw std::runtime_error("Unexpected token: " + std::to_string(token.op));
            }
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
        ASTNodePtr expr;
        if(tokens[index].concept == TOKEN::TOKEN_CONCEPTS::FUNCTION) {
            index++;
            expr = parseBlock();
        } else {

        expr = parseExpression();
        }

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

    index++;
    if(index < tokens.size() && tokens[index].concept == TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS) {
        return handleFunctionRefrence();
    }
    index--;

    SymbolTable& currentTable = *symbolTableStack.top();
    const TOKEN& token = tokens[index];

    if (token.concept != TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME) {
        throw std::runtime_error("Expected variable name");
    }

    std::string varName = token.variableName;
    index++;

    
    if (index < tokens.size() && 
        (tokens[index].op == TOKEN::OPERATORS::EQUALS_OPERATOR || 
         tokens[index].op == TOKEN::OPERATORS::INCREMENT_OPERATOR || 
         tokens[index].op == TOKEN::OPERATORS::DECREMENT_OPERATOR || 
         tokens[index].op == TOKEN::OPERATORS::NOT_OPERATOR) ||
         tokens[index].concept == TOKEN::TOKEN_CONCEPTS::OPEN_SQUARE_BRACKETS || tokens[index].concept == TOKEN::TOKEN_CONCEPTS::ACSESS) {

        
        auto op = tokens[index].op;
        
        

    
        if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
            index++;
        }

        
        if (op == TOKEN::OPERATORS::EQUALS_OPERATOR) {
            index++;
        auto expr = parseExpression();
            currentTable.setVariableValue(varName, std::move(expr));
        } else if (op == TOKEN::OPERATORS::INCREMENT_OPERATOR) {
            index++;
        } else if (op == TOKEN::OPERATORS::DECREMENT_OPERATOR) {
            index++;
        } else if (op == TOKEN::OPERATORS::NOT_OPERATOR) {
            index++;
        } else if(tokens[index].concept == TOKEN::TOKEN_CONCEPTS::OPEN_SQUARE_BRACKETS)
        {
            index++;
            ASTNodePtr indexExpr = parseLogicalExpression();
            index++;
            if(tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_SQUARE_BRACKETS) {
                std::runtime_error("Need '['");
            }
            
            return std::make_unique<ArrayAccessNode>(varName,std::move(indexExpr));
            
            
        } else if (tokens[index].concept == TOKEN::TOKEN_CONCEPTS::ACSESS) {
    index++;

    
    std::string property = tokens[index].variableName;

    
    auto classNodePtr = findClassInSymbolTableStack(varName, *symbolTableStack.top());
    auto objectNodePtr = findVariableInSymbolTableStack(varName, *symbolTableStack.top());


    auto classNode = static_cast<ClassNode*>(classNodePtr.get());
    auto objectNode = static_cast<ObjectNode*>(objectNodePtr.get());

    if (classNode) {
        
        std::string method;
        std::string attribute;

        
        if (tokens[index + 1].concept == TOKEN::OPEN_CIRCLE_BRACKETS) {
            method = property;
        } else {
            attribute = property;
        }

        return std::make_unique<ClassAccessNode>(
    attribute,
    method,
    classNode->clone()
);




    } else if (objectNode) {
        
        
        std::string method;
        std::string attribute;

        
        if (tokens[index + 1].concept == TOKEN::OPEN_CIRCLE_BRACKETS) {
            method = property;
        } else {
            attribute = property;
            if(tokens[index+1].op == TOKEN::OPERATORS::EQUALS_OPERATOR) {
                auto value = parseLogicalExpression();
                objectNode->setField(attribute, std::make_unique<VariableNode>(attribute, std::move(value)));
                return std::make_unique<EmptyNode>();
            }
        }

        return std::make_unique<ObjectAccessNode>(attribute, method, std::unique_ptr<ObjectNode>(objectNode));



    } else {
        throw std::runtime_error("Unknown class or object for access.");
    }
}

        return AST::makeEmptyNode();
    }
    
    ASTNodePtr variableValueNode = findVariableInSymbolTableStack(varName, currentTable);

    
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
    } else if(token.concept == TOKEN::TOKEN_CONCEPTS::CONTINUE) {
        return parseContinue();
    } else if(token.concept == TOKEN::TOKEN_CONCEPTS::BREAK) {
        return parseBreak();
    }
    else {
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

ASTNodePtr Parser::parseBreak() {
    index++;
    
    
    if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
        index++;
        return std::make_unique<BreakNode>();
    } else {
        std::cerr << "Error: Unexpected token after 'break' at index " << index << std::endl;
        return nullptr;
    }
}

ASTNodePtr Parser::parseContinue() {
    index++; 
    
    
    if (index < tokens.size() && tokens[index].op == TOKEN::OPERATORS::NEWLINE_OPERATOR) {
        index++;
        return std::make_unique<ContinueNode>();
    } else {
        std::cerr << "Error: Unexpected token after 'continue' at index " << index << std::endl;
        return nullptr;
    }
}





ASTNodePtr Parser::parseFor() {
    auto node = std::make_unique<ForNode>(nullptr, nullptr, nullptr, nullptr);

    
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS) {
        std::cerr << "Error: 'for' statement needs an opening '('." << std::endl;
        throw std::runtime_error("Expected '(' after 'for'");
    }
    index++;

    
    node->initialization = parseLogicalExpression();

    
    if (tokens[index].concept == TOKEN::TOKEN_CONCEPTS::SEMICOLON) {
        index++;
    } else {
        std::cerr << "Error: Expected ';' after initialization in 'for' loop." << std::endl;
        throw std::runtime_error("Expected ';' after initialization in 'for' loop");
    }

    
    node->expression = parseLogicalExpression();

    
    if (tokens[index].concept == TOKEN::TOKEN_CONCEPTS::SEMICOLON) {
        index++;
    } else {
        std::cerr << "Error: Expected ';' after condition in 'for' loop." << std::endl;
        throw std::runtime_error("Expected ';' after condition in 'for' loop");
    }

    
    node->increment = parseLogicalExpression();


    if (tokens[index].concept == TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE_BRACKETS) {
        index++; 
    } else {
        std::cerr << "Error: Expected ')' after increment in 'for' loop." << std::endl;
        throw std::runtime_error("Expected ')' after increment in 'for' loop");
    }


    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        std::cerr << "Error: 'for' loop needs an opening '{' for the block." << std::endl;
        throw std::runtime_error("Expected '{' to start 'for' loop block");
    }


    node->forBlock = parseBlock();

    return node;
}

ASTNodePtr Parser::parseArray()
{
    std::vector<ASTNodePtr> array;

    while (index < tokens.size() && tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_SQUARE_BRACKETS)
    {
        ASTNodePtr expression = parseLogicalExpression();
        array.push_back(std::move(expression));

        if (index < tokens.size() && tokens[index].concept == TOKEN::TOKEN_CONCEPTS::COMMA)
        {
            ++index;
        }
    }
    if (index >= tokens.size() || tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_SQUARE_BRACKETS) {
        throw std::runtime_error("Expected closing square bracket.");
    }
    ++index;
    ASTNodePtr arrayNode = std::make_unique<ArrayNode>(std::move(array));

    bool containsWhileNode = false;
    bool containsForNode = false;

    for (const auto& node : array) {
        if (static_cast<WhileNode*>(node.get())) {
            containsWhileNode = true;
        }
        if (static_cast<ForNode*>(node.get())) {
            containsForNode = true;
        }
    }

    if (containsWhileNode) {
        throw std::runtime_error("Can't have while");
    }

    if (containsForNode) {
        throw std::runtime_error("Can't have for");
    }

    return arrayNode;
}


ASTNodePtr Parser::parseFunction() {
    index++;
    if (index >= tokens.size() || tokens[index].concept != TOKEN::TOKEN_CONCEPTS::FUNCTION_NAME) {
        std::cerr << "Expected function name but got a different token" << std::endl;
        throw std::runtime_error("Expected function name");
    }

    SymbolTable& currentTable = *FunctionTableStack.top();

    const TOKEN& token = tokens[index];
    std::string functionName = token.variableName;
    index++;
    std::vector<ASTNodePtr> argument_list;

    if(index<tokens.size() && tokens[index].concept == TOKEN::TOKEN_CONCEPTS::OPEN_CIRCLE_BRACKETS) {
        index++;
        while(index<tokens.size() && tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE_BRACKETS) {
            auto var = parseLogicalExpression();
            argument_list.push_back(std::move(var));
        }
    }
    index++;

    if (index < tokens.size() && tokens[index].concept == TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        ASTNodePtr expr = parseBlock();
        currentTable.setVariableValue(functionName, std::make_unique<FunctionNode>(std::move(expr), functionName, std::move(argument_list)));
        return AST::makeEmptyNode();
    } else {
        throw std::runtime_error("Expected function name");
    }
    return AST::makeEmptyNode();
}

ASTNodePtr Parser::handleFunctionRefrence() {
    index--;
    if (index >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens");
    }
    SymbolTable& currentTable = *FunctionTableStack.top();
    const TOKEN& token = tokens[index];
    std::string functionName = token.variableName;

    std::vector<ASTNodePtr> argument_list;

    
        index++;
        index++;
        while(index<tokens.size() && tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_CIRCLE_BRACKETS) {
            auto var = parseLogicalExpression();
            argument_list.push_back(std::move(var));
        }
        index++;

    ASTNodePtr functionASTNode = findFunctionInSymbolTableStack(functionName, currentTable);
    FunctionNode* functionNode = static_cast<FunctionNode*>(functionASTNode.get());



    
    if (!functionNode) {
        std::cerr << "Undefined function: " << functionName << std::endl;
        throw std::runtime_error("Undefined function: " + functionName);
    }


    return std::make_unique<FunctionNode>(std::move(functionNode->block), functionName, std::move(argument_list));
}


ASTNodePtr Parser::findVariableInSymbolTableStack(const std::string& varName, SymbolTable& currentTable) {

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

ASTNodePtr Parser::findFunctionInSymbolTableStack(const std::string& functionName, SymbolTable& currentTable) {
    
    ASTNodePtr functionNode = currentTable.getVariableValue(functionName);
    if (functionNode) {
        return functionNode;
    }

    
    std::stack<std::unique_ptr<SymbolTable>> tempStack;
    bool found = false;

    
    while (!FunctionTableStack.empty()) {
        std::unique_ptr<SymbolTable> tempTable = std::move(FunctionTableStack.top());  // Move the unique_ptr
        FunctionTableStack.pop();  
        tempStack.push(std::move(tempTable));  

        functionNode = tempStack.top()->getVariableValue(functionName);
        if (functionNode) {
            found = true;
            break;
        }
    }

 
    while (!tempStack.empty()) {
        FunctionTableStack.push(std::move(tempStack.top()));  
        tempStack.pop();
    }

    if (!found) {
        std::cerr << "Undefined variable: " << functionName << std::endl;
        throw std::runtime_error("Undefined variable: " + functionName);
    }

    return functionNode;
}


ASTNodePtr Parser::parseClass() {
    index++;
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLASS_NAME) {
        throw std::runtime_error("Expected class name after 'class'");
    }

    std::string className = tokens[index].variableName;
    std::unique_ptr<SymbolTable> methods = std::make_unique<SymbolTable>();
    std::unique_ptr<SymbolTable> attributes = std::make_unique<SymbolTable>();
    std::unique_ptr<AST> constructor = nullptr;  
    std::unique_ptr<AST> parentClass = nullptr;

    index++;

    if(tokens[index].concept == TOKEN::TOKEN_CONCEPTS::EXTENDS) {
        index++;
        if(tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLASS_NAME) {
            parentClass = findClassInSymbolTableStack(tokens[index].variableName,*classTableStack.top());
        }
        index++;
    }

    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::OPEN_BRACKETS) {
        throw std::runtime_error("Expected '{'");
    }

    index++;
    while (tokens.size() > index && tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS) {
        if (tokens[index].variableName == className && tokens[index + 1].concept == TOKEN::OPEN_CIRCLE_BRACKETS) {

            constructor = parseMethodOrConstructor();  
        } else if(tokens[index].concept == TOKEN::TOKEN_CONCEPTS::SUPER)
        {
            return parseSuperExpression(parentClass);
        }
        else {
            parseMethodAndProperty(methods, attributes);
        }
        if(tokens[index].concept == TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS) {
            break;
        }
        index++;
    }

    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLOSE_BRACKETS) {
        throw std::runtime_error("Expected '}'");
    }
    index++;    

    auto res = std::make_unique<ClassNode>(std::move(methods), std::move(attributes), std::move(constructor));
    if(parentClass) {
        res->inheritFrom(std::move(static_cast<ClassNode*>(parentClass.get())));
    }
    
    classTableStack.top()->setVariableValue(className,res->clone());
    return res;
}

ASTNodePtr Parser::parseSuperExpression(ASTNodePtr &currentClassNode) {
    
    index++;
    
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::ACSESS) {
        throw std::runtime_error("Expected '.' after 'super'");
    }
    
    index++;
    
    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::VARIABLE_NAME) {
        throw std::runtime_error("Expected method name after 'super.'");
    }
    
    std::string methodName = tokens[index].variableName;
    index++; 
    
    return std::make_unique<Super_Expr>(currentClassNode, methodName);  
}



ASTNodePtr Parser::findClassInSymbolTableStack(const std::string& className, SymbolTable& currentTable) {
    
    ASTNodePtr classNode = currentTable.getVariableValue(className);
    if (classNode) {
        return classNode;
    }

    
    std::stack<std::unique_ptr<SymbolTable>> tempStack;
    bool found = false;

    
    while (!classTableStack.empty()) {
        std::unique_ptr<SymbolTable> tempTable = std::move(classTableStack.top());
        classTableStack.pop();  
        tempStack.push(std::move(tempTable));  

        classNode = tempStack.top()->getVariableValue(className);
        if (classNode) {
            found = true;
            break;
        }
    }

 
    while (!tempStack.empty()) {
        classTableStack.push(std::move(tempStack.top()));  
        tempStack.pop();
    }

    if (!found) {
        std::cerr << "Undefined variable: " << className << std::endl;
        throw std::runtime_error("Undefined variable: " + className);
    }

    return classNode;
}

ASTNodePtr Parser::parseMethodAndProperty(std::unique_ptr<SymbolTable> &methods, std::unique_ptr<SymbolTable> &attributes) {
    if (tokens[index].concept == TOKEN::VARIABLE_NAME && tokens[index + 1].concept != TOKEN::OPEN_CIRCLE_BRACKETS) {
        
        std::string varName = tokens[index].variableName;
        ASTNodePtr varNode = std::make_unique<VariableNode>(varName, nullptr);
        attributes->setVariableValue(varName, std::move(varNode));
        // index++;
        return varNode;
    } else if (tokens[index].concept == TOKEN::VARIABLE_NAME && tokens[index + 1].concept == TOKEN::OPEN_CIRCLE_BRACKETS) {
        
        std::string functionName = tokens[index].variableName;
        index++;
        
        std::vector<ASTNodePtr> argument_list;
        if (tokens[index].concept == TOKEN::OPEN_CIRCLE_BRACKETS) {
            index++;
            while (tokens[index].concept != TOKEN::CLOSE_CIRCLE_BRACKETS) {
                auto arg = parseLogicalExpression();
                argument_list.push_back(std::move(arg));
            }
            index++;
        }

        ASTNodePtr functionBody = parseBlock();
        methods->setVariableValue(functionName, std::move(functionBody));
    }
}



ASTNodePtr Parser::parseMethodOrConstructor() {
    std::string functionName = tokens[index].variableName;
    index++;
    
    std::vector<ASTNodePtr> argument_list;
    if (tokens[index].concept == TOKEN::OPEN_CIRCLE_BRACKETS) {
        index++;
        while (tokens[index].concept != TOKEN::CLOSE_CIRCLE_BRACKETS) {
            argument_list.push_back(parseLogicalExpression());
        }
        index++;
    }

    ASTNodePtr functionBody = parseBlock(); 
    return std::make_unique<FunctionNode>(std::move(functionBody), std::move(functionName), std::move(argument_list));

}

ASTNodePtr Parser::parseObjectInstance() {

    if (tokens[index].concept != TOKEN::TOKEN_CONCEPTS::CLASS_NAME) {
        throw std::runtime_error("can't use new without class name");
    }

    
    auto className = tokens[index].variableName;
    index++;

    
    auto res = findClassInSymbolTableStack(className, *classTableStack.top());
    if (!res) {
        throw std::runtime_error("this is not a class name");
    }

    auto classNode = static_cast<ClassNode*>(res.get());
    if (!classNode) {
        throw std::runtime_error("Class node not found.");
    }

    
    std::vector<ASTNodePtr> argument_list;
    if (tokens[index].concept == TOKEN::OPEN_CIRCLE_BRACKETS) {
        index++;
        while (tokens[index].concept != TOKEN::CLOSE_CIRCLE_BRACKETS) {
            auto arg = parseLogicalExpression();
            argument_list.push_back(std::move(arg));
        }
        index++;
    }
    index++;

    
    auto objectNode = std::make_unique<ObjectNode>(res->clone());

    
    if (classNode->constructor) {
        auto constructor = static_cast<FunctionNode*>(classNode->constructor.get());
        if (constructor) {
            constructor->argument_list = std::move(argument_list);
    
        }
    }
    

    return std::move(objectNode); 
}
