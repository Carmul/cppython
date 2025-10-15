#include "interpreter.h"
#include "builtInFunctions.h"
#include <string>


void Interpreter::printVariables() const {
	std::cout << "------------------\n";
    std::cout << "Current Variables:\n";
    for (const auto& [name, value] : variables) {
        std::cout << "  " << name << " = " << value.toString() << "\n";
    }
    std::cout << "------------------\n";
}

void Interpreter::registetrBuiltins() {
    builtins["print"] = printFunc;
	builtins["println"] = printlnFunc;
	builtins["max"] = maxFunc;
	builtins["min"] = minFunc;
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

    else if (node.op == "==") {
        if (!(lresult.isString() || rresult.isString()))
			result = Value(lresult.asNumber() == rresult.asNumber());
        else if (lresult.isString() && rresult.isString())
            result = Value(lresult.asString() == rresult.asString());
        else
            throw std::runtime_error("Type error in '==': cannot compare " + lresult.typeName() + " and " + rresult.typeName());
    }
    else if (node.op == "!=") {
        if (!(lresult.isString() || rresult.isString()))
            result = Value(lresult.asNumber() != rresult.asNumber());
        else if (lresult.isString() && rresult.isString())
            result = Value(lresult.asString() != rresult.asString());
        else
            throw std::runtime_error("Type error in '!=': cannot compare " + lresult.typeName() + " and " + rresult.typeName());
    }
    else if (node.op == "<") {
        if (!(lresult.isString() || rresult.isString()))
            result = Value(lresult.asNumber() < rresult.asNumber());
        else
            throw std::runtime_error("Type error in '<': cannot compare " + lresult.typeName() + " and " + rresult.typeName());
    }
    else if (node.op == "<=") {
        if (!(lresult.isString() || rresult.isString()))
            result = Value(lresult.asNumber() <= rresult.asNumber());
        else
            throw std::runtime_error("Type error in '<=': cannot compare " + lresult.typeName() + " and " + rresult.typeName());
    }
    else if (node.op == ">") {
        if (!(lresult.isString() || rresult.isString()))
            result = Value(lresult.asNumber() > rresult.asNumber());
        else
            throw std::runtime_error("Type error in '>': cannot compare " + lresult.typeName() + " and " + rresult.typeName());
    }
    else if (node.op == ">=") {
        if (!(lresult.isString() || rresult.isString()))
            result = Value(lresult.asNumber() >= rresult.asNumber());
        else
            throw std::runtime_error("Type error in '>=': cannot compare " + lresult.typeName() + " and " + rresult.typeName());
    }
    else if (node.op == "and") {
        result = Value(lresult.isTruthy() && rresult.isTruthy());
    }
    else if (node.op == "or") {
        result = Value(lresult.isTruthy() || rresult.isTruthy());
	}
    else if (node.op == "%") {
        result = Value(static_cast<double>(static_cast<int>(lresult.asNumber()) % static_cast<int>(rresult.asNumber())));
	}
    else {
        throw std::runtime_error("Unknown binary operator: " + node.op);
	}
}

void Interpreter::visit(const UnaryOpNode& node) {
	node.factor->accept(*this);
    if (node.op == "+")
        ;// unary plus, do nothing
	else if (node.op == "-")
		result = Value(- result.asNumber());
	else if (node.op == "not")
        result = Value(! result.isTruthy());
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

void Interpreter::visit(const BlockNode& node) {
    for (const auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void Interpreter::visit(const IfNode& node) {
    node.condition->accept(*this);
    Value cond = result;
    if (cond.isTruthy()) {
		node.body->accept(*this);
    }
    else {
		if (node.elseBody)
		    node.elseBody->accept(*this);
	}
}

void Interpreter::visit(const WhileNode& node) {
    node.condition->accept(*this);
    Value cond = result;
    while (cond.isTruthy()) {
        node.body->accept(*this);

        node.condition->accept(*this);
        cond = result;
    }
}

void Interpreter::visit(const FunctionCallNode& node) {
	std::string funcName = node.funcName;
    callStack.push_back(funcName);
	std::vector<Value> args;
    for (const auto& argNode : node.arguments) {
        argNode->accept(*this);
        args.push_back(result);
	}

	// Handle built-in functions
    if(builtins.find(funcName) != builtins.end()) {
        result = builtins[funcName](args);
		callStack.pop_back();
        return;
	}
    
	// Handle user-defined functions
    if (definedFunctions.find(funcName) != definedFunctions.end()) {
        auto funcDef = definedFunctions[funcName].get();
        if (args.size() != funcDef->parameters.size()) {
            throw std::runtime_error("Function '" + funcName + "' expects " + std::to_string(funcDef->parameters.size()) + " arguments, got " + std::to_string(args.size()));
        }
        // Save current variables to restore later (simple scope handling)
        auto savedVariables = variables;
        // Set function parameters
        for (size_t i = 0; i < args.size(); ++i) {
            variables[funcDef->parameters[i]] = args[i];
        }
        // Execute function body
        funcDef->body->accept(*this);
        // Restore previous variables
        variables = savedVariables;
		callStack.pop_back();
		return;
    }
    
	throw std::runtime_error("Function '" + funcName + "' not defined");

}

void Interpreter::visit(FunctionDefNode& node) {

	// check if function already defined
	if (definedFunctions.find(node.funcName) != definedFunctions.end()) {
		throw std::runtime_error("Function '" + node.funcName + "' already defined");
	}

    definedFunctions[node.funcName] = std::make_unique<FunctionDefNode>(
        node.funcName,
        node.parameters,
        std::move(node.body)
    );
    
}

