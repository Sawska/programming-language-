#include "ObjectAccessNode.h"

ObjectAccessNode::ObjectAccessNode(std::string memberName,std::string methodName, std::unique_ptr<AST> baseObject)
    : AST(Type::ObjectAccess), memberName(std::move(memberName)), methodName(std::move(methodName)), baseObject(std::move(baseObject)) {}


    std::unique_ptr<AST> ObjectAccessNode::clone() const {
    return std::make_unique<ObjectAccessNode>(
        memberName,
        methodName,
        baseObject->clone()
    );
}
