#include "interpreter.h"
#include <string>


void Interpreter::visit(const NumberNode& node) {
	result = std::stod(node.value);
}

void Interpreter::visit(const BinaryOpNode& node) {
    node.left->accept(*this);
    double lval = result;
    node.right->accept(*this);
    double rval = result;

    if (node.op == "+") 
        result = lval + rval;
    else if (node.op == "-")
        result = lval - rval;
    else if (node.op == "*")
        result = lval * rval;
    else if (node.op == "/") 
        result = lval / rval;
}

void Interpreter::visit(const UnaryOpNode& node) {
	node.factor->accept(*this);
    if (node.op == "+")
        ;// unary plus, do nothing
	else if (node.op == "-")
		result = -result;
}