#ifndef CLASSACCESSNODE_H
#define CLASSACCESSNODE_H

#include "AST.h"
#include <string>

class ClassAccessNode : public AST {
public:
    std::string memberName; 
    std::string methodName;
    ASTNodePtr baseClass;

    ClassAccessNode(std::string memberName,std::string methodName, ASTNodePtr baseClass);

    ASTNodePtr clone() const override;
};

#endif // CLASSACCESSNODE_H
