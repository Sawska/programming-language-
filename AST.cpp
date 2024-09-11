#include "AST.h"
#include "SymbolTable.h"
#include "NumberNode.h"
#include "BinaryOperationNode.h"
#include "UnaryOperationNode.h"
#include "StringNode.h"
#include "EmptyNode.h"
#include "VariableNode.h"

AST::AST(Type type) : type(type), value(0), op(TOKEN::OPERATORS::UNKNOWN), left(nullptr), right(nullptr) {}

ASTNodePtr AST::makeNumberNode(double value) {
    return std::make_unique<NumberNode>(value);
}

ASTNodePtr AST::makeBinaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr left, ASTNodePtr right) {
    return std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
}

ASTNodePtr AST::makeUnaryOperationNode(TOKEN::OPERATORS op, ASTNodePtr operand) {
    return std::make_unique<UnaryOperationNode>(op, std::move(operand));
}

ASTNodePtr AST::makeStringNode(const std::string& value) {
    return std::make_unique<StringNode>(value);
}

ASTNodePtr AST::makeEmptyNode() {
    return std::make_unique<EmptyNode>();
}

ASTNodePtr AST::makeSequenceNode(ASTNodePtr left, ASTNodePtr right) {
    return std::make_unique<BinaryOperationNode>(TOKEN::OPERATORS::SEQUENCE_OPERATOR, std::move(left), std::move(right));

}

ASTNodePtr AST::makeVariableNode(const std::string &name, ASTNodePtr value)
{
     return std::make_unique<VariableNode>(name, std::move(value));
}
