#ifndef NUMBERNODE_H

#define NUMBERNODE_H

#include "AST.h"

class NumberNode : public AST {
public:
    NumberNode(double value);
    double value;
    std::unique_ptr<AST> clone() const override;
};

#endif //NUMBERNODE_H