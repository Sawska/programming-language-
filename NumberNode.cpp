#include "NumberNode.h"

NumberNode::NumberNode(double value) : AST(Type::Number), value(value) {}

std::unique_ptr<AST> NumberNode::clone() const {
    return std::make_unique<NumberNode>(value);
}
