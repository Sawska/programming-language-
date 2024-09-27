#include "ClassAccessNode.h"

ClassAccessNode::ClassAccessNode(std::string memberName,std::string methodName, ASTNodePtr baseClass)
    : AST(Type::ClassAccess), memberName(std::move(memberName)), methodName(std::move(methodName)), baseClass(std::move(baseClass)) {}

ASTNodePtr ClassAccessNode::clone() const {
    return std::make_unique<ClassAccessNode>(
        memberName,
        methodName,
        baseClass->clone()
    );
}
