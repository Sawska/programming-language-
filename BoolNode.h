#ifndef BOOLNODE_H

#define BOOLNODE_H

#include "AST.h"

class BoolNode : public AST {
public:
    bool type;
    BoolNode(bool type);
    std::unique_ptr<AST> clone() const override;
};

#endif //BOOLNODE_H