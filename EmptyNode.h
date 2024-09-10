#ifndef EMPTYNODE_H

#define EMPTYNODE_H 

#include "AST.h"

class EmptyNode : public AST {
public:
    EmptyNode();
    std::unique_ptr<AST> clone() const override;
};

#endif //EMPTYNODE_H