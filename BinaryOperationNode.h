#ifndef BINARY_OPERATION_NODE

#define BINARY_OPERATION_NODE

#include "AST.h"

class BinaryOperationNode : public AST {
public:
    BinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right);
    std::unique_ptr<AST> clone() const override;
     TOKEN::OPERATORS op;
    ASTNodePtr left;
    ASTNodePtr right;
};
#endif //BINARY_OPERATION_NODE