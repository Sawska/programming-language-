#include "AST.h"

ASTNodePtr AST::makeNumberNode(double value) {
    return std::make_unique<NumberNode>(value);
}



ASTNodePtr AST::makeUnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand) {
    return std::make_unique<UnaryOperationNode>(op, std::move(operand));
}

ASTNodePtr AST::makeStringNode(const std::string& value) {
    return std::make_unique<StringNode>(value);
}


ASTNodePtr AST::makeBinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right) {
    return std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
}

ASTNodePtr AST::makeEmptyNode() {
    return std::make_unique<EmptyNode>();
}