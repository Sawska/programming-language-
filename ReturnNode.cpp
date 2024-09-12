#include "ReturnNode.h"

ReturnNode::ReturnNode(ASTNodePtr value) 
    : AST(Type::Return),value(std::move(value)) {}

std::unique_ptr<AST> ReturnNode::clone() const {
    return std::make_unique<ReturnNode>(value->clone());
}