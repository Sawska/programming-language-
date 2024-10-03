#ifndef SUPER_EXPR_H
#define SUPER_EXPR_H

#include "AST.h"
#include "Token.h"
#include <memory>

class Super_Expr : public AST {
public:
    ASTNodePtr baseClass;
    std::string method; 

    Super_Expr(ASTNodePtr baseClass, std::string method); 
    
};

#endif // SUPER_EXPR_H
