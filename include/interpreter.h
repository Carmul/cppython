#include "ast.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <variant>


// Value type that can hold different types
class Value {
public:
    using ValueType = std::variant<double, bool, std::string>;

private:
    ValueType data;

public:
    // Constructors
    Value() : data(0.0) {}
    Value(double d) : data(d) {}
    Value(bool b) : data(b) {}
    Value(const std::string& s) : data(s) {}
    Value(const char* s) : data(std::string(s)) {}

    // Type checking
    bool isNumber() const { return std::holds_alternative<double>(data); }
    bool isBool() const { return std::holds_alternative<bool>(data); }
    bool isString() const { return std::holds_alternative<std::string>(data); }

    // Type accessors with error checking
    double asNumber() const {
        if (auto* val = std::get_if<double>(&data)) {
            return *val;
        }
        if (auto* val = std::get_if<bool>(&data)) {
            return *val ? 1.0 : 0.0;
		}
        throw std::runtime_error("Value is not a number");
    }

    bool asBool() const {
        if (auto* val = std::get_if<bool>(&data)) {
            return *val;
        }
        throw std::runtime_error("Value is not a boolean");
    }

    std::string asString() const {
        if (auto* val = std::get_if<std::string>(&data)) {
            return *val;
        }
        throw std::runtime_error("Value is not a string");
    }

    // Convert to string for printing
    std::string toString() const {
        if (isNumber()) {
            return std::to_string(asNumber());
        }
        else if (isBool()) {
            return asBool() ? "True" : "False";
        }
        else if (isString()) {
            return asString();
        }
        return "undefined";
    }

    // Type name for error messages
    std::string typeName() const {
        if (isNumber()) return "number";
        if (isBool()) return "bool";
        if (isString()) return "string";
        return "unknown";
    }

    // Truthiness (for conditions)
    bool isTruthy() const {
        if (isBool()) return asBool();
        if (isNumber()) return asNumber() != 0.0;
        if (isString()) return !asString().empty();
        return false;
    }
};



class Interpreter : public Visitor {
public:
	ASTNodePtr tree;

	Interpreter(ASTNodePtr t) : result(0.0), tree(std::move(t)) {}

	Value interpret();

	void printVariables() const;

	void visit(const NumberNode& node) override;
	void visit(const BinaryOpNode& node) override;
	void visit(const UnaryOpNode& node) override;
	void visit(const ProgramNode& node) override;
	void visit(const PrintNode& node) override;
	void visit(const VarNode& node) override;
	void visit(const AssignmentNode& node) override;
	void visit(const BooleanNode& node) override;
	void visit(const StringNode& node) override;
		
private:
	std::unordered_map<std::string, Value> variables;
    Value result;
};

