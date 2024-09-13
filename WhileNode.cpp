#include "WhileNode.h"

WhileNode::WhileNode(ASTNodePtr expression,ASTNodePtr WhileBlock) : AST(Type::WHILE), expression(std::move(expression)), WhileBlock(std::move(WhileBlock)) {}

std::unique_ptr<AST> WhileNode::clone() const  {
    return std::make_unique<WhileNode>(expression->clone(), WhileBlock->clone());
}

