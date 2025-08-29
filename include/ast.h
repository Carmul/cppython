#pragma once

#include <iostream>
#include <memory>
#include <vector>

class Visitor;
class NumberNode;
class BinaryOpNode;
class UnaryOpNode;
class ProgramNode;
class PrintNode;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::string toString() const = 0;
    virtual std::string getNodeType() const = 0;

    virtual void accept(Visitor& v) const = 0; // accept a visitor
};

// Visitor interface for AST nodes
class Visitor {
public:
    virtual void visit(const NumberNode& node) = 0;
    virtual void visit(const BinaryOpNode& node) = 0;
	virtual void visit(const UnaryOpNode& node) = 0;
	virtual void visit(const ProgramNode& node) = 0;
	virtual void visit(const PrintNode& node) = 0;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

// Number node (terminals)
class NumberNode : public ASTNode {
public:
    std::string value;

    NumberNode(const std::string val);
    std::string toString() const override;
    std::string getNodeType() const override;

    void accept(Visitor& v) const override {
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

    void accept(Visitor& v) const override {
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

    void accept(Visitor& v) const override {
        v.visit(*this);
    }
};

// Program node (root of the AST)
class ProgramNode : public ASTNode {
public:
    std::vector<ASTNodePtr> statements;

    ProgramNode(std::vector<ASTNodePtr> stmts);
    std::string toString() const override;
    std::string getNodeType() const override;

    void accept(Visitor& v) const override {
        v.visit(*this);
    }
};

class PrintNode : public ASTNode {
public:
	ASTNodePtr expr;

    PrintNode(ASTNodePtr e);
    std::string toString() const override;
    std::string getNodeType() const override;
	void accept(Visitor& v) const override {
		v.visit(*this);
	}
};