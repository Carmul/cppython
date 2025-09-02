#include "dotGenerator.h"


std::string DotGenerator::generate(const ASTNodePtr root) {
	dot = "digraph G {\n";
	// traverse the AST and build the dot representation
	root->accept(*this);
	dot += "}\n";
	return dot;
}

void DotGenerator::visit(const UnaryOpNode& node) {
	std::string id = newId();
	dot += "    " + id + " [label=\"" + node.getNodeType() + "[" + node.op + "]" + "\"];\n";
	node.factor->accept(*this);
	std::string factorId = stack.back().id; stack.pop_back();
	dot += "    " + id + " -> " + factorId + ";\n";
	stack.push_back({ id });
}

void DotGenerator::visit(const NumberNode& node) {
    std::string id = newId();
    dot += "    " + id + " [label=\"" + node.getNodeType() + "[" + node.value + "]" + "\"];\n";
    stack.push_back({ id });
}

void DotGenerator::visit(const BinaryOpNode& node) {
    std::string id = newId();
    dot += "    " + id + " [label=\"" + node.getNodeType() + "[" + node.op + "]" + "\"];\n";

    node.left->accept(*this);
    std::string leftId = stack.back().id; stack.pop_back();
    dot += "    " + id + " -> " + leftId + ";\n";

    node.right->accept(*this);
    std::string rightId = stack.back().id; stack.pop_back();
    dot += "    " + id + " -> " + rightId + ";\n";

    stack.push_back({ id });
}

void DotGenerator::visit(const ProgramNode& node) {
	std::string id = newId();
	dot += "    " + id + " [label=\"" + node.getNodeType() + "\"];\n";
	for (const auto& stmt : node.statements) {
		stmt->accept(*this);
		std::string stmtId = stack.back().id; stack.pop_back();
		dot += "    " + id + " -> " + stmtId + ";\n";
	}
	stack.push_back({ id });
}

void DotGenerator::visit(const PrintNode& node) {
	std::string id = newId();
	dot += "    " + id + " [label=\"" + node.getNodeType() + "\"];\n";
	node.expr->accept(*this);
	std::string exprId = stack.back().id; stack.pop_back();
	dot += "    " + id + " -> " + exprId + ";\n";
	stack.push_back({ id });
}

void DotGenerator::visit(const VarNode& node) {
	std::string id = newId();
	dot += "    " + id + " [label=\"" + node.getNodeType() + "[" + node.name + "]" + "\"];\n";
	stack.push_back({ id });
}

void DotGenerator::visit(const AssignmentNode& node) {
	std::string id = newId();
	dot += "    " + id + " [label=\"" + node.getNodeType() + "\"];\n";
	// Variable node
	std::string varId = newId();
	dot += "    " + varId + " [label=\"Var[" + node.varNode->toString() + "]\"];\n";
	dot += "    " + id + " -> " + varId + ";\n";
	// Value expression
	node.value->accept(*this);
	std::string valueId = stack.back().id; stack.pop_back();
	dot += "    " + id + " -> " + valueId + ";\n";
	stack.push_back({ id });
}