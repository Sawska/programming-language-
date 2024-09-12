#ifndef RETURNNODE_H

#define RETURNNODE_H

#include "AST.h"

class ReturnNode : public AST {
public:
    ReturnNode(ASTNodePtr value);
    ASTNodePtr value;
    std::unique_ptr<AST> clone() const override;
};

#endif //RETURNNODE_H