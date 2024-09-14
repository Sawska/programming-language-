#include "ContinueNode.h"

ContinueNode::ContinueNode() : AST(Type::Continue) {}

std::unique_ptr<AST> ContinueNode::clone() const {
    return std::make_unique<ContinueNode>();
}

