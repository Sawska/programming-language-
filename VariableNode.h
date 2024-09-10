#ifndef VARIABLENODE_H

#define  VARIABLENODE_H

#include "AST.h"

class VariableNode : public AST {
public:
    std::string name;
    ASTNodePtr value;
    VariableNode(std::string name, ASTNodePtr value);
    std::unique_ptr<AST> clone() const override;
};

#endif // VARIABLENODE_H