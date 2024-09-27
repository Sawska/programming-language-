#include "ClassNode.h"

ClassNode::ClassNode(std::unique_ptr<SymbolTable> methods, 
                     std::unique_ptr<SymbolTable> attributes, 
                     std::unique_ptr<AST> constructor)
    : AST(Type::Class), 
      methods(std::move(methods)), 
      attributes(std::move(attributes)),
      constructor(std::move(constructor)),
      parentClass(nullptr) {}

std::unique_ptr<AST> ClassNode::clone() const {
    return std::make_unique<ClassNode>(
        methods->clone(), 
        attributes->clone(), 
        constructor ? constructor->clone() : nullptr  
    );
}

void ClassNode::inheritFrom(ClassNode* parent) {
    if (parent) {

        for (const auto& [name, member] : parent->attributes->table) {
            attributes->setVariableValue(name, member->clone());
        }
        for (const auto& [name, method] : parent->methods->table) {
            methods->setVariableValue(name, method->clone());
        }
        parentClass = parent;
    }
}

ASTNodePtr ClassNode::getField(const std::string& fieldName) {
    
    auto value = attributes->getVariableValue(fieldName);
    if (value) {
        return value;
    }
    
    if (parentClass) {
        return parentClass->getField(fieldName);
    }
    throw std::runtime_error("Field '" + fieldName + "' not found.");
}

ASTNodePtr ClassNode::getMethod(const std::string& methodName) {
    
    auto method = methods->getVariableValue(methodName);
    if (method) {
        return method;
    }
    
    if (parentClass) {
        return parentClass->getMethod(methodName);
    }
    throw std::runtime_error("Method not found: " + methodName);
}
