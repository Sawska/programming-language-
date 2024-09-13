#include "IfNode.h"

 IfNode::IfNode(ASTNodePtr ifBlock, ASTNodePtr expression, ASTNodePtr elseBlock)
        : AST(Type::IF), ifBlock(std::move(ifBlock)), expression(std::move(expression)), elseBlock(std::move(elseBlock)) {}


std::unique_ptr<AST> IfNode::clone() const  {
    return std::make_unique<IfNode>(ifBlock->clone(), expression->clone(), elseBlock ? elseBlock->clone() : nullptr);
}
