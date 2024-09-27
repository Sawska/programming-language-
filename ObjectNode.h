#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "AST.h"
#include "SymbolTable.h"
#include <stdexcept>  
#include "ClassNode.h"

class ObjectNode : public AST {
public:
    std::string className;
    ASTNodePtr classNode; 
    std::unique_ptr<SymbolTable> attributes;
    std::unique_ptr<SymbolTable> methods;
    void setMethod(const std::string& methodName, ASTNodePtr method);
    ASTNodePtr getMethod(const std::string& methodName);

    
    ObjectNode(const std::string& name, ASTNodePtr node);

    
    void setField(const std::string& fieldName, ASTNodePtr value);
    ASTNodePtr getField(const std::string& fieldName);

    
    std::unique_ptr<AST> clone() const override;
};

#endif // OBJECTNODE_H
