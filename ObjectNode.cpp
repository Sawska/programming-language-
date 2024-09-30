#include "ObjectNode.h"

ObjectNode::ObjectNode( ASTNodePtr node)
    : AST(AST::Type::Object),  classNode(std::move(node)) {
    attributes = std::make_unique<SymbolTable>(); 
    methods = std::make_unique<SymbolTable>();    
}

void ObjectNode::setField(const std::string& fieldName, ASTNodePtr value) {
    attributes->setVariableValue(fieldName, std::move(value));
}

ASTNodePtr ObjectNode::getField(const std::string& fieldName) {
    auto value = attributes->getVariableValue(fieldName);
    auto classNodePtr = static_cast<ClassNode*>(classNode.get());

    if (value) {
        return value;
    } else if (classNodePtr && classNodePtr->parentClass) {
        return classNodePtr->parentClass->getField(fieldName);
    }

    throw std::runtime_error("Field '" + fieldName + "' not found.");
}

std::unique_ptr<AST> ObjectNode::clone() const {


    auto clonedClassNode = classNode->clone();


    auto clonedNode = std::make_unique<ObjectNode>(std::move(clonedClassNode));


    
    clonedNode->attributes = std::make_unique<SymbolTable>();
    clonedNode->attributes->table = this->attributes->deepCopyTable(); 
    clonedNode->methods = std::make_unique<SymbolTable>();
    clonedNode->methods->table = this->methods->deepCopyTable(); 
    
    return clonedNode;
}


void ObjectNode::setMethod(const std::string& methodName, ASTNodePtr method) {
    methods->setVariableValue(methodName, std::move(method));
}

ASTNodePtr ObjectNode::getMethod(const std::string& methodName) {
    auto result = methods->getVariableValue(methodName);
    auto classNodePtr = static_cast<ClassNode*>(classNode.get()); 

    if (result) {
        return result;
    } else if (classNodePtr && classNodePtr->parentClass) {
        
        return classNodePtr->parentClass->getMethod(methodName);
    }

    throw std::runtime_error("Method not found: " + methodName);
}
