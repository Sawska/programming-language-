#ifndef ARRAYNODE_H
#define ARRAYNODE_H

#include "AST.h"
#include <vector>
#include <memory>

class ArrayNode : public AST {
public:
    int size;
    std::vector<ASTNodePtr> array;

    ArrayNode(std::vector<ASTNodePtr> elements);

    std::unique_ptr<AST> clone() const override;
};

#endif // ARRAYNODE_H
