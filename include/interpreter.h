#include "ast.h"
#include <string>
#include <memory>

class Interpreter : public Visitor {
public:
	ASTNodePtr tree;

	Interpreter(ASTNodePtr t) : result(0), tree(std::move(t)) {}

	double interpret();

	void visit(const NumberNode& node) override;
	void visit(const BinaryOpNode& node) override;
	void visit(const UnaryOpNode& node) override;
	void visit(const ProgramNode& node) override;
	void visit(const PrintNode& node) override;
		
private:
	double result;

};