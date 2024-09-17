#ifndef ARRAYACCESSNODE_H
#define ARRAYACCESSNODE_H

#include "AST.h"
#include <memory>
#include <string>

class ArrayAccessNode : public AST {
public:
    std::string variableName;     
    ASTNodePtr indexExpression;   


    ArrayAccessNode(std::string varName, ASTNodePtr indexExpr);


    std::unique_ptr<AST> clone() const override;
};

#endif // ARRAYACCESSNODE_H
