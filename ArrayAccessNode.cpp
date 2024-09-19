#include "ArrayAccessNode.h"

ArrayAccessNode::ArrayAccessNode(std::string varName, ASTNodePtr indexExpr)
    : AST(AST::Type::Access), variableName(std::move(varName)), indexExpression(std::move(indexExpr)) {}

std::unique_ptr<AST> ArrayAccessNode::clone() const {
    return std::make_unique<ArrayAccessNode>(variableName, indexExpression->clone());
}
