#include "ForNode.h"

ForNode::ForNode(ASTNodePtr init, ASTNodePtr expr, ASTNodePtr incr, ASTNodePtr block) : AST(Type::FOR), initialization(std::move(init)), expression(std::move(expr)), forBlock(std::move(block)), increment(std::move(incr)) {}


std::unique_ptr<AST> ForNode::clone() const  {
    return std::make_unique<ForNode>(initialization->clone(),expression->clone(),increment->clone(),forBlock->clone());
}

