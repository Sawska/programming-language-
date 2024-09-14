#ifndef BREAKNODE_H

#define BREAKNODE_H

#include "AST.h"

class BreakNode : public AST {
    public:
    std::unique_ptr<AST> clone() const override;
    BreakNode();
};

#endif //BREAKNODE_H