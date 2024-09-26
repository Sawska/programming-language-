#ifndef CLASSACCESSNODE_H
#define CLASSACCESSNODE_H

#include "AST.h"
#include <string>

class ClassAccessNode : public AST {
public:
    std::string memberName; 
    std::string methodName;
    std::unique_ptr<AST> baseClass;

    ClassAccessNode(std::string memberName,std::string methodName, std::unique_ptr<AST> baseClass);

    std::unique_ptr<AST> clone() const override;
};

#endif // CLASSACCESSNODE_H
