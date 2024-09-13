#ifndef WHILENODE_H

#define WHILENODE_H

#include "AST.h"

class WhileNode : public AST {
    public: 
        ASTNodePtr expression;
        ASTNodePtr WhileBlock;
        std::unique_ptr<AST> clone() const override;
        WhileNode(ASTNodePtr expression,ASTNodePtr WhileBlock);
};

#endif //WHILENODE_H