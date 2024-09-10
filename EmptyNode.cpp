#include "EmptyNode.h"

EmptyNode::EmptyNode() : AST(Type::Empty) {}

std::unique_ptr<AST> EmptyNode::clone() const {
    return std::make_unique<EmptyNode>();
}
