#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include "token.h"
#include "SymbolTable.h"

class AST;

using ASTNodePtr = std::unique_ptr<AST>;

class AST {
public:
    enum class Type {
        Number,
        BinaryOperation,
        UnaryOperation,
        ParenthesizedExpression,
        Variable,
        VariableExpression,
        String,  
        Unknown,
    };

    Type type;
    double value;             
    std::string stringValue;  
    TOKEN::OPERATORS op;
    ASTNodePtr left;
    ASTNodePtr right;

    explicit AST(Type type)
        : type(type), value(0), op(TOKEN::OPERATORS::UNKNOWN), left(nullptr), right(nullptr) {}

    virtual ~AST() = default;

    
    static ASTNodePtr makeNumberNode(double value);
    static ASTNodePtr makeBinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right);
    static ASTNodePtr makeUnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand);
    static ASTNodePtr makeStringNode(const std::string& value);  
};


class VariableNode : public AST {
public:
    std::string name;
    ASTNodePtr value;

    VariableNode(std::string name, ASTNodePtr value)
        : AST(Type::Variable), name(std::move(name)), value(std::move(value)) {}

    TOKEN evaluate(const SymbolTable& symbolTable) const {
        return symbolTable.getVariableValue(name);
    }
};


class ExpressionNode : public AST {
public:
    ExpressionNode(Type type) : AST(type) {}
};

#endif // AST_H
