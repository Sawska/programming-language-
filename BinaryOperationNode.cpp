#include "BinaryOperationNode.h"

BinaryOperationNode::BinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right)
    : AST(Type::BinaryOperation), op(op), left(std::move(left)), right(std::move(right)) {}

std::unique_ptr<AST> BinaryOperationNode::clone() const {
    return std::make_unique<BinaryOperationNode>(op, left->clone(), right->clone());
}
