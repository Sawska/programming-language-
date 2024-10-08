#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>
#include "token.h"



using ASTNodePtr = std::unique_ptr<class AST>;

/**
 * @file AST.h
 * @brief Header file for the AST class.
 * 
 * This file contains the definition of the AST class,
 * which handles  all operations in programming language.
 */

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
        Block,
        Empty,
        Return,
        IF,
        WHILE,
        Break,
        Continue,
        FOR,
        Array,
        Access,
        Function,
        Bool,
        Class,
        ClassAccess,
        Object,
        ObjectAccess,
        Super,
    };

    Type type;
    double value;
    std::string stringValue;
    TOKEN::OPERATORS op;
    ASTNodePtr left;
    ASTNodePtr right;

    explicit AST(Type type);
    AST(AST&&) = default;
    AST& operator=(AST&&) = default;
    virtual ~AST() = default;
    
    virtual std::unique_ptr<AST> clone() const = 0;

    static ASTNodePtr makeNumberNode(double value);
    static ASTNodePtr makeBinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right);
    static ASTNodePtr makeUnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand);
    static ASTNodePtr makeStringNode(const std::string& value);
    static ASTNodePtr makeEmptyNode();
    static ASTNodePtr makeSequenceNode(ASTNodePtr left, ASTNodePtr right);
    static ASTNodePtr makeVariableNode(const std::string& name, ASTNodePtr value);
};


#endif // AST_H