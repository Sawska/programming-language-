#include "SuperExpr.h"

Super_Expr::Super_Expr(ASTNodePtr baseClass, std::string method)
    : AST(Type::Super), baseClass(std::move(baseClass)), method(std::move(method)) {}

