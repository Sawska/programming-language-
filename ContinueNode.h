#ifndef CONTINUENODE_H

#define CONTINUENODE_H

#include "AST.h"

class ContinueNode : public AST {
    public:
    std::unique_ptr<AST> clone() const override;
    ContinueNode();
};

#endif //CONTINUENODE_H