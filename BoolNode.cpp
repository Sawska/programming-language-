#include "BoolNode.h"

BoolNode::BoolNode(bool type) : AST(Type::Bool), type(type)  {}

std::unique_ptr<AST> BoolNode::clone() const {
    return std::make_unique<BoolNode>(type);
}