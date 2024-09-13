#ifndef IFNODE_H

#define IFNODE_H

#include "AST.h"

class IfNode : public AST {
    public:
    ASTNodePtr expression;
    ASTNodePtr ifBlock;
    ASTNodePtr elseBlock;
    std::unique_ptr<AST> clone() const override;
    IfNode( ASTNodePtr ifBlock,ASTNodePtr expression,ASTNodePtr elseBlock = nullptr);
};


#endif //IFNODE_H