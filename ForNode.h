#ifndef FORNODE_H
#define FORNODE_H

#include "AST.h"
#include "WhileNode.h"

class ForNode : public AST {
public:
    ASTNodePtr initialization;  
    ASTNodePtr expression;      
    ASTNodePtr increment;       
    ASTNodePtr forBlock;        

    ForNode(ASTNodePtr init, ASTNodePtr expr, ASTNodePtr incr, ASTNodePtr block);
    std::unique_ptr<AST> clone() const override;
};

#endif // FORNODE_H
