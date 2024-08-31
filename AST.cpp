#include <AST.h>

ASTNodePtr AST::makeNumberNode(double value)
{
    auto node = std::make_unique<AST>(AST::Type::Number);
    node->value = value;
    return node;
}

ASTNodePtr AST::makeBinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right) {
  auto node = std::make_unique<AST>(AST::Type::BinaryOperation);
    node->op = op;
    node->left = std::move(left);
    node->right = std::move(right);
    return node;
}

ASTNodePtr AST::makeUnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand) {
    auto node = std::make_unique<AST>(AST::Type::UnaryOperation);
    node->op = op;
    node->left = std::move(operand);
    return node;
}