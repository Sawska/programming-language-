#include "FunctionNode.h"

FunctionNode::FunctionNode(ASTNodePtr block, std::string name,std::vector<ASTNodePtr> argument_list)
    : AST(AST::Type::Function), block(std::move(block)), name(std::move(name)), argument_list(std::move(argument_list)) {
}

std::unique_ptr<AST> FunctionNode::clone() const {
    std::vector<ASTNodePtr> clonedElements;
    clonedElements.reserve(argument_list.size());

    for (const auto& element : argument_list) {
        clonedElements.push_back(element->clone()); 
    }
    
    return std::make_unique<FunctionNode>(block->clone(), std::string(name), std::move(clonedElements));
}
