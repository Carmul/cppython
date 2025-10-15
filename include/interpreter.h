#pragma once

#include "ast.h"
#include "value.h"
#include "builtInFunctions.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <variant>




class Interpreter : public Visitor {
public:
	ASTNodePtr tree;
	std::vector<std::string> callStack;

	Interpreter(ASTNodePtr t) : result(0.0), tree(std::move(t)) { registetrBuiltins();  }

	Value interpret();

	void printVariables() const;

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
	std::unordered_map<std::string, Value> variables;
    Value result;
	std::unordered_map<std::string, BuiltinFunc> builtins;
	std::unordered_map<std::string, std::unique_ptr<FunctionDefNode>> definedFunctions;

	void registetrBuiltins();
};

