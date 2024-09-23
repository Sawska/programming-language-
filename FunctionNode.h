#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "AST.h"
#include <string>
#include <vector>

class FunctionNode : public AST {
public:
    ASTNodePtr block;  
    std::vector<ASTNodePtr> argument_list;
    std::string name;


    FunctionNode(ASTNodePtr block, std::string name,std::vector<ASTNodePtr> argument_list);
    

    std::unique_ptr<AST> clone() const override;
};

#endif // FUNCTIONNODE_H
