#include "ast.h"


// NumberNode class
NumberNode::NumberNode(const std::string val) : value(val) {}

std::string NumberNode::toString() const {
    return value;
}

std::string NumberNode::getNodeType() const { return "Number"; }


// BinaryOpNode class
BinaryOpNode::BinaryOpNode(ASTNodePtr l, const std::string& operation, ASTNodePtr r)
    : left(std::move(l)), op(operation), right(std::move(r)) {}

std::string BinaryOpNode::toString() const {
    return "{" + left->toString() + " " + op + " " + right->toString() + "}";
}

std::string BinaryOpNode::getNodeType() const { return "BinaryOp"; }


// UnaryOpNode class
UnaryOpNode::UnaryOpNode(const std::string& operation, ASTNodePtr factorNode)
	: op(operation), factor(std::move(factorNode)) {}

std::string UnaryOpNode::toString() const {
	return "{" + op + factor->toString() + "}";
}

std::string UnaryOpNode::getNodeType() const { return "UnaryOp"; }


// ProgramNode class
ProgramNode::ProgramNode(std::vector<ASTNodePtr> stmts) : statements(std::move(stmts)) {}

std::string ProgramNode::toString() const {
	std::string result = "Program:\n";
	for (const auto& stmt : statements) {
		result += "  " + stmt->toString() + "\n";
	}
	return result;
}

std::string ProgramNode::getNodeType() const { return "Program"; }


// PrintNode class
PrintNode::PrintNode(ASTNodePtr expr) : expr(std::move(expr)) {}

std::string PrintNode::toString() const {
	return "Print(" + expr->toString() + ")";
}

std::string PrintNode::getNodeType() const { return "Print"; }
