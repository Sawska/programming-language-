#include "FunctionNode.h"

FunctionNode::FunctionNode(ASTNodePtr block, std::string name)
    : AST(AST::Type::Function), block(std::move(block)), name(std::move(name)) {
}

std::unique_ptr<AST> FunctionNode::clone() const {
    return std::make_unique<FunctionNode>(block->clone(), name);  
}
