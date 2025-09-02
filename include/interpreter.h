#include "ast.h"
#include <string>
#include <memory>
#include <unordered_map>

class Interpreter : public Visitor {
public:
	ASTNodePtr tree;

	Interpreter(ASTNodePtr t) : result(0), tree(std::move(t)) {}

	double interpret();

	void printVariables() const;

	void visit(const NumberNode& node) override;
	void visit(const BinaryOpNode& node) override;
	void visit(const UnaryOpNode& node) override;
	void visit(const ProgramNode& node) override;
	void visit(const PrintNode& node) override;
	void visit(const VarNode& node) override;
	void visit(const AssignmentNode& node) override;
		
private:
	double result = 6969;
	std::unordered_map<std::string, double> variables;
};