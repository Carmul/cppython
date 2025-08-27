#pragma once

#include <iostream>
#include <memory>

class Visitor;
class NumberNode;
class BinaryOpNode;

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
