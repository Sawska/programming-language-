#include "ArrayNode.h"

ArrayNode::ArrayNode(std::vector<ASTNodePtr> elements) 
    : AST(AST::Type::Array), size(elements.size()), array(std::move(elements)) {
}

std::unique_ptr<AST> ArrayNode::clone() const {
    std::vector<ASTNodePtr> clonedElements;
    clonedElements.reserve(array.size());
    for (const auto& element : array) {
        clonedElements.push_back(std::move(element->clone())); 
    }
return std::make_unique<ArrayNode>(std::move(clonedElements))
}
