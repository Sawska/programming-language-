#include "BreakNode.h"

BreakNode::BreakNode() : AST(Type::Break) {}

std::unique_ptr<AST> BreakNode::clone() const {
    return std::make_unique<BreakNode>();
}

