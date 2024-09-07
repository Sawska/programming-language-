#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include "token.h"

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
        Empty
    };

    Type type;
    double value;
    std::string stringValue;
    TOKEN::OPERATORS op;
    ASTNodePtr left;
    ASTNodePtr right;

    explicit AST(Type type)
        : type(type), value(0), op(TOKEN::OPERATORS::UNKNOWN), left(nullptr), right(nullptr) {}

    AST(AST&&) = default;
    AST& operator=(AST&&) = default;
    virtual ~AST() = default;

    virtual std::unique_ptr<AST> clone() const = 0;

    static ASTNodePtr makeNumberNode(double value);
    static ASTNodePtr makeBinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right);
    static ASTNodePtr makeUnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand);
    static ASTNodePtr makeStringNode(const std::string& value);
    static ASTNodePtr makeEmptyNode();
};

class NumberNode : public AST {
public:
    NumberNode(double value)
        : AST(Type::Number) {
        this->value = value;
    }

    std::unique_ptr<AST> clone() const override {
        return std::make_unique<NumberNode>(value);
    }
};

class BinaryOperationNode : public AST {
public:
    BinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right)
        : AST(Type::BinaryOperation) {
        this->op = op;
        this->left = std::move(left);
        this->right = std::move(right);
    }

    std::unique_ptr<AST> clone() const override {
        return std::make_unique<BinaryOperationNode>(op, left ? left->clone() : nullptr, right ? right->clone() : nullptr);
    }
};

class UnaryOperationNode : public AST {
public:
    UnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand)
        : AST(Type::UnaryOperation) {
        this->op = op;
        this->left = std::move(operand);
    }

    std::unique_ptr<AST> clone() const override {
        return std::make_unique<UnaryOperationNode>(op, left ? left->clone() : nullptr);
    }
};

class StringNode : public AST {
public:
    StringNode(const std::string& value)
        : AST(Type::String) { 
        this->stringValue = value;
    }

    std::unique_ptr<AST> clone() const override {
        return std::make_unique<StringNode>(stringValue);
    }
};
class VariableNode : public AST {
public:
    std::string name;
    ASTNodePtr value;

    VariableNode(std::string name, ASTNodePtr value)
        : AST(Type::Variable), name(std::move(name)), value(std::move(value)) {}

    std::unique_ptr<AST> clone() const override {
        return std::make_unique<VariableNode>(name, value ? value->clone() : nullptr);
    }
};

class EmptyNode : public AST {
public:
    EmptyNode() : AST(Type::Empty) {} 

    std::unique_ptr<AST> clone() const override {
        return std::make_unique<EmptyNode>();
    }
};



#endif // AST_H
