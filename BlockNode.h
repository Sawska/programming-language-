#ifndef BLOCKNODE_H

#define BLOCKNODE_H


#include "AST.h"
#include "SymbolTable.h"

class SymbolTable; 

class BlockNode : public AST {
public:
    BlockNode();
    std::unique_ptr<AST> clone() const override;
    SymbolTable* getSymbolTable() const;
    void addStatement(ASTNodePtr stmt);
    const std::vector<ASTNodePtr>& getStatements() const;

private:
    std::unique_ptr<SymbolTable> symbol_table;
    std::vector<ASTNodePtr> statements;
};

#endif //BLOCKNODE_H