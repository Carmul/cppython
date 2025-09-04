#include "interpreter.h"
#include <string>


void Interpreter::printVariables() const {
	std::cout << "------------------\n";
    std::cout << "Current Variables:\n";
    for (const auto& [name, value] : variables) {
        std::cout << "  " << name << " = " << value.toString() << "\n";
    }
    std::cout << "------------------\n";
}


Value Interpreter::interpret() {
    if (tree) {
        tree->accept(*this);
    }
    else {
        throw std::runtime_error("No AST to interpret");
    }
    return result;
}


void Interpreter::visit(const NumberNode& node) {
	result = Value(std::stod(node.value));
}

void Interpreter::visit(const BinaryOpNode& node) {
    node.left->accept(*this);
    Value lresult = result;
    node.right->accept(*this);
    Value rresult = result;

    if (node.op == "+") {
		if (lresult.isString() && rresult.isString()) { // string concatenation
            result = Value(lresult.asString() + rresult.asString());
            return;
		}

        result = Value(lresult.asNumber() + rresult.asNumber());
    }
    else if (node.op == "-") {
        result = Value(lresult.asNumber() - rresult.asNumber());
    }
    else if (node.op == "*") {
		if (lresult.isString() && rresult.isNumber()) { // string repetition
            int times = static_cast<int>(rresult.asNumber());
            if (times < 0) times = 0;
            std::string repeated;
            for (int i = 0; i < times; ++i) {
                repeated += lresult.asString();
            }
            result = Value(repeated);
            return;
        }
		else if (lresult.isNumber() && rresult.isString()) { // string repetition
            int times = static_cast<int>(lresult.asNumber());
            if (times < 0) times = 0; // prevent negative repetitions
            std::string repeated;
            for (int i = 0; i < times; ++i) {
                repeated += rresult.asString();
            }
            result = Value(repeated);
            return;
		}

        result = Value(lresult.asNumber() * rresult.asNumber());
    }
    else if (node.op == "/") {
        result = Value(lresult.asNumber() / rresult.asNumber());
    }

}

void Interpreter::visit(const UnaryOpNode& node) {
	node.factor->accept(*this);
    if (node.op == "+")
        ;// unary plus, do nothing
	else if (node.op == "-")
		result = Value(- result.asNumber());
    
}

void Interpreter::visit(const ProgramNode& node) {
	for (const auto& stmt : node.statements) {
		stmt->accept(*this);
	}
}

void Interpreter::visit(const PrintNode& node) {
	node.expr->accept(*this);
	std::cout << result.toString() << std::endl;
}

void Interpreter::visit(const VarNode& node) {
    if (variables.find(node.toString()) == variables.end()) {
		std::cerr << "Error: Variable '" << node.toString() << "' not defined." << std::endl;
        throw std::runtime_error("Variable '" + node.toString() + "' not defined");
	}
	result = variables[node.toString()];
}

void Interpreter::visit(const AssignmentNode& node) {
    node.value->accept(*this);
	variables[node.varNode->toString()] = result;
}

void Interpreter::visit(const BooleanNode& node) {
	if (node.value == "True")
		result = Value(true);
	else
        result = Value(false);
}

void Interpreter::visit(const StringNode& node) {
    result = Value(node.value);
}
