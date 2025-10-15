#pragma once

#include <iostream>
#include <memory>
#include <vector>

class Visitor;
class NumberNode;
class BinaryOpNode;
class UnaryOpNode;
class VarNode;
class AssignmentNode;
class BooleanNode;
class StringNode;
class BlockNode;
class IfNode;
class WhileNode;
class FunctionCallNode;
class FunctionDefNode;
class ReturnNode;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::string toString() const = 0;
    virtual std::string getNodeType() const = 0;

    virtual void accept(Visitor& v) = 0; // accept a visitor
};

// Visitor interface for AST nodes
class Visitor {
public:
    virtual void visit(const NumberNode& node) = 0;
    virtual void visit(const BinaryOpNode& node) = 0;
	virtual void visit(const UnaryOpNode& node) = 0;
	virtual void visit(const VarNode& node) = 0;
	virtual void visit(const AssignmentNode& node) = 0;
	virtual void visit(const BooleanNode& node) = 0;
	virtual void visit(const StringNode& node) = 0;
	virtual void visit(const BlockNode& node) = 0;
	virtual void visit(const IfNode& node) = 0;
	virtual void visit(const WhileNode& node) = 0;
	virtual void visit(const FunctionCallNode& node) = 0;
	virtual void visit(FunctionDefNode& node) = 0;
	virtual void visit(ReturnNode& node) = 0;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

// Number node (terminals)
class NumberNode : public ASTNode {
public:
    std::string value;

    NumberNode(const std::string val);
    std::string toString() const override;
    std::string getNodeType() const override;

    void accept(Visitor& v) override {
        v.visit(*this);
    }
};

// Binary operation node (for now only +, -, *, /)
class BinaryOpNode : public ASTNode {
public:
    ASTNodePtr left;
    std::string op;
    ASTNodePtr right;

    BinaryOpNode(ASTNodePtr l, const std::string& operation, ASTNodePtr r);
    std::string toString() const override;
    std::string getNodeType() const override;

    void accept(Visitor& v) override {
        v.visit(*this);
    }
};

// Unary operation node (for now only +, -)
class UnaryOpNode : public ASTNode {
public:
	std::string op;
	ASTNodePtr factor;

    UnaryOpNode(const std::string& operation, ASTNodePtr e);
    std::string toString() const override;
    std::string getNodeType() const override;

    void accept(Visitor& v) override {
        v.visit(*this);
    }
};


class VarNode : public ASTNode {
public:
    std::string name;

    VarNode(const std::string n);
    std::string toString() const override;
    std::string getNodeType() const override;

    void accept(Visitor& v) override {
        v.visit(*this);
    }
};

class AssignmentNode : public ASTNode {
public:
    ASTNodePtr varNode;
    ASTNodePtr value;

    AssignmentNode(ASTNodePtr var, ASTNodePtr val) : varNode(std::move(var)), value(std::move(val)) {}

    std::string toString() const override {
        return varNode->toString() + " = " + value->toString();
    }

    std::string getNodeType() const override { return "Assignment"; }

    void accept(Visitor& v) override {
        v.visit(*this);
    }

};

class BooleanNode : public ASTNode {
public:
    std::string value;

    BooleanNode(std::string val) : value(val) {}

    std::string toString() const override {
        return value;
    }

    std::string getNodeType() const override { return "Boolean"; }

    void accept(Visitor& v) override {
        v.visit(*this);
    }
};

class StringNode : public ASTNode {
    public:
    std::string value;

    StringNode(std::string val) : value(val) {}

    std::string toString() const override {
        return "\"" + value + "\"";
    }
    std::string getNodeType() const override { return "String"; }

    void accept(Visitor& v) override {
        v.visit(*this);
	}
};

class BlockNode : public ASTNode {
    public:
    std::vector<ASTNodePtr> statements;

    BlockNode(std::vector<ASTNodePtr> stmts) : statements(std::move(stmts)) {}

    std::string toString() const override {
        std::string result = "Block:\n";
        for (const auto& stmt : statements) {
            result += "  " + stmt->toString() + "\n";
        }
        return result;
    }

    std::string getNodeType() const override { return "Block"; }
    void accept(Visitor& v) override {
        v.visit(*this);
	}
};

class IfNode : public ASTNode {
public:
    ASTNodePtr condition;
    ASTNodePtr body;
	ASTNodePtr elseBody; // Optional else body

    IfNode(ASTNodePtr cond, ASTNodePtr b, ASTNodePtr eb = nullptr) : condition(std::move(cond)), body(std::move(b)), elseBody(std::move(eb)) {}

    std::string toString() const override {
        return "If " + condition->toString() + ":\n" + body->toString();
    }
    std::string getNodeType() const override { return "If"; }
    void accept(Visitor& v) override {
        v.visit(*this);
    }
};

class WhileNode : public ASTNode {
    public:
    ASTNodePtr condition;
    ASTNodePtr body;

    WhileNode(ASTNodePtr cond, ASTNodePtr b) : condition(std::move(cond)), body(std::move(b)) {}

    std::string toString() const override {
        return "While " + condition->toString() + ":\n" + body->toString();
    }

    std::string getNodeType() const override { return "While"; }

    void accept(Visitor& v) override {
        v.visit(*this);
	}
};

class FunctionCallNode : public ASTNode {
    public:
    std::string funcName;
    std::vector<ASTNodePtr> arguments;
    FunctionCallNode(const std::string& name, std::vector<ASTNodePtr> args) : funcName(name), arguments(std::move(args)) {}
    std::string toString() const override {
        std::string argsStr;
        for (const auto& arg : arguments) {
            if (!argsStr.empty()) argsStr += ", ";
            argsStr += arg->toString();
        }
        return funcName + "(" + argsStr + ")";
    }
    std::string getNodeType() const override { return "FunctionCall"; }

    void accept(Visitor& v) override {
        v.visit(*this);
    }
};

class FunctionDefNode : public ASTNode {
public:
    std::string funcName;
    std::vector<std::string> parameters;
    ASTNodePtr body;

    FunctionDefNode(const std::string& name, std::vector<std::string> params, ASTNodePtr b)
        : funcName(name), parameters(std::move(params)), body(std::move(b)) {}

 
    std::string toString() const override {
        std::string paramsStr;
        for (const auto& param : parameters) {
            if (!paramsStr.empty()) paramsStr += ", ";
            paramsStr += param;
        }
        return "def " + funcName + "(" + paramsStr + "):\n" + body->toString();
    }

    std::string getNodeType() const override { return "FunctionDef"; }

    void accept(Visitor& v) override {
        v.visit(*this);
    }


};

class ReturnNode : public ASTNode {
    public:
    ASTNodePtr value;
    ReturnNode(ASTNodePtr val) : value(std::move(val)) {}
    std::string toString() const override {
        return "return " + value->toString();
    }
    std::string getNodeType() const override { return "Return"; }
    void accept(Visitor& v) override {
		v.visit(*this);
	}
};