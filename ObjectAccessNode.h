#ifndef OBJECT_ACCSESS_NODE_H

#define OBJECT_ACCSESS_NODE_H

#include "AST.h"

class ObjectAccessNode : public AST {
public:
    std::string memberName;
    std::string methodName;
    std::unique_ptr<AST> baseObject;

    ObjectAccessNode(std::string memberName,std::string methodName, std::unique_ptr<AST> baseObject);

    std::unique_ptr<AST> clone() const override;
};

#endif //OBJECT_ACCSESS_NODE_H