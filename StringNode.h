#ifndef STRINGNODE

#define STRINGNODE
#include "AST.h"


class StringNode : public AST {
public:
    StringNode(const std::string& value);
    const std::string& stringValue;
    std::unique_ptr<AST> clone() const override;
};

#endif //STRINGNODE