#ifndef AST_H

#define AST_H

#include <token.h>
#include <memory>

class AST;

using ASTNodePtr = std::unique_ptr<AST>;

 class AST {
    public:
     enum class Type {
        Number,
        BinaryOperation,
        UnaryOperation,
        ParenthesizedExpression
    };

    Type type;

    
    double value;

    
    TOKEN::OPERATORS op;
    ASTNodePtr left;
    ASTNodePtr right;

    AST(Type type) : type(type), value(0), op(TOKEN::OPERATORS::UNKNOWN) {}

static     ASTNodePtr makeNumberNode(double value);
 static   ASTNodePtr makeBinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right);
static     ASTNodePtr makeUnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand);

};

#endif //AST_H