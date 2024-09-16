#include "UnaryOperationNode.h"

UnaryOperationNode::UnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand)
    : AST(Type::UnaryOperation), op(op), operand(std::move(operand)) {}

std::unique_ptr<AST> UnaryOperationNode::clone() const {
    return std::make_unique<UnaryOperationNode>(op, left->clone());
}
