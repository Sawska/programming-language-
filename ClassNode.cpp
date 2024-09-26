#include "ClassNode.h"

ClassNode::ClassNode(std::unique_ptr<SymbolTable> methods, 
                     std::unique_ptr<SymbolTable> attributes, 
                     std::unique_ptr<AST> constructor)
    : AST(Type::Class), 
      methods(std::move(methods)), 
      attributes(std::move(attributes)),
      constructor(std::move(constructor)) {}

std::unique_ptr<AST> ClassNode::clone() const {
    return std::make_unique<ClassNode>(
        methods->clone(), 
        attributes->clone(), 
        constructor ? constructor->clone() : nullptr  
    );
}
