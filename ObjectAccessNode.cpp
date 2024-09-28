#include "ObjectAccessNode.h"

ObjectAccessNode::ObjectAccessNode(std::string memberName,std::string methodName, ASTNodePtr baseObject)
    : AST(Type::ObjectAccess), memberName(std::move(memberName)), methodName(std::move(methodName)), baseObject(std::move(baseObject)) {}


    ASTNodePtr ObjectAccessNode::clone() const {
    return std::make_unique<ObjectAccessNode>(
        memberName,
        methodName,
        baseObject->clone()
    );
}
