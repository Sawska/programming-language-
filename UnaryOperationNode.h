#ifndef UNARY_OPERATION_NODE_H

#define UNARY_OPERATION_NODE_H

#include "AST.h"

class UnaryOperationNode : public AST {
public:
    UnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand);
    std::unique_ptr<AST> clone() const override;
    TOKEN::OPERATORS op;
    ASTNodePtr operand;
};

#endif //UNARY_OPERATION_NODE_H