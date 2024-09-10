#include "BlockNode.h"

BlockNode::BlockNode() : AST(Type::Block), symbol_table(std::make_unique<SymbolTable>()) {}

std::unique_ptr<AST> BlockNode::clone() const {
    auto cloned_node = std::make_unique<BlockNode>();
    cloned_node->symbol_table = std::make_unique<SymbolTable>();
    cloned_node->symbol_table->table = this->symbol_table->deepCopyTable(); 
    for (const auto& stmt : statements) {
        cloned_node->statements.push_back(stmt->clone());
    }
    return cloned_node;
}

SymbolTable* BlockNode::getSymbolTable() const {
    return symbol_table.get();
}

void BlockNode::addStatement(ASTNodePtr stmt) {
    statements.push_back(std::move(stmt));
}

const std::vector<ASTNodePtr>& BlockNode::getStatements() const {
    return statements;
}



