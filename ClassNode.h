#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "AST.h"
#include "SymbolTable.h"

class ClassNode : public AST {
public:
    std::unique_ptr<SymbolTable> methods;
    std::unique_ptr<SymbolTable> attributes;

    std::unique_ptr<AST> clone() const override;

    ClassNode(std::unique_ptr<SymbolTable> methods, std::unique_ptr<SymbolTable> attributes);
};

#endif // CLASSNODE_H
