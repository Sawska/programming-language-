#include "VariableNode.h"

VariableNode::VariableNode(std::string name, ASTNodePtr value)
    : AST(Type::Variable), name(std::move(name)), value(std::move(value)) {}

std::unique_ptr<AST> VariableNode::clone() const {
    if(value == nullptr) 
    {
        return std::make_unique<VariableNode>(name, nullptr);    
    }
    return std::make_unique<VariableNode>(name, value->clone());
}
