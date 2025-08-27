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

// more class to come...