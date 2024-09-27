#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "AST.h"
#include "SymbolTable.h"
#include <memory>

class ClassNode : public AST {
public:
    std::unique_ptr<SymbolTable> methods;
    std::unique_ptr<SymbolTable> attributes;
    std::unique_ptr<AST> constructor;
    ClassNode* parentClass;

    std::unique_ptr<AST> clone() const override;

    ClassNode(std::unique_ptr<SymbolTable> methods, 
              std::unique_ptr<SymbolTable> attributes, 
              std::unique_ptr<AST> constructor);

    void inheritFrom(ClassNode* parent);


    ASTNodePtr getField(const std::string& fieldName);
    ASTNodePtr getMethod(const std::string& methodName);
};

#endif // CLASSNODE_H
