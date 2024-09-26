#include "ClassAccessNode.h"

ClassAccessNode::ClassAccessNode(std::string memberName,std::string methodName, std::unique_ptr<AST> baseClass)
    : AST(Type::ClassAccess), memberName(std::move(memberName)), methodName(std::move(methodName)), baseClass(std::move(baseClass)) {}

std::unique_ptr<AST> ClassAccessNode::clone() const {
    return std::make_unique<ClassAccessNode>(
        memberName,
        methodName,
        baseClass->clone()
    );
}
