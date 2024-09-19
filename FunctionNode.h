#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "AST.h"
#include <string>

class FunctionNode : public AST {
public:
    ASTNodePtr block;  
    std::string name;


    FunctionNode(ASTNodePtr block, std::string name);
    

    std::unique_ptr<AST> clone() const override;
};

#endif // FUNCTIONNODE_H
