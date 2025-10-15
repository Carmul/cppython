#pragma once

#include "ast.h"
#include <string>
#include <vector>

class DotGenerator : Visitor {
public:
    std::string generate(const ASTNodePtr root);

    void visit(const NumberNode& node) override;
    void visit(const BinaryOpNode& node) override;
    void visit(const UnaryOpNode& node) override;
	void visit(const VarNode& node) override;
	void visit(const AssignmentNode& node) override;
	void visit(const BooleanNode& node) override;
	void visit(const StringNode& node) override;
	void visit(const BlockNode& node) override;
	void visit(const IfNode& node) override;
	void visit(const WhileNode& node) override;
    void visit(const FunctionCallNode& node) override;
    void visit(FunctionDefNode& node) override;

private:
    std::string dot;
    int counter = 0;

    struct NodeId {
        std::string id;
    };

    std::vector<NodeId> stack;

    std::string newId() {
        return "n" + std::to_string(counter++);
    }

};