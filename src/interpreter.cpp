#include "interpreter.h"
#include <string>


double Interpreter::interpret() {
    if (tree) {
        tree->accept(*this);
    }
    else {
        throw std::runtime_error("No AST to interpret");
    }
    return result;
}


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

void Interpreter::visit(const ProgramNode& node) {
	for (const auto& stmt : node.statements) {
        //result = 0;
		stmt->accept(*this);
	}
}


void Interpreter::visit(const PrintNode& node) {
	node.expr->accept(*this);
	std::cout << "((stdout)) " << result << std::endl;
}