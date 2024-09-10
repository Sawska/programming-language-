#include "StringNode.h"

StringNode::StringNode(const std::string& value) : AST(Type::String), stringValue(value) {}

std::unique_ptr<AST> StringNode::clone() const {
    return std::make_unique<StringNode>(stringValue);
}
