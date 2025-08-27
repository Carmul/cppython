#include "ast.h"
#include <string>

class Interpreter : public Visitor {
public:
    double result;

    void visit(const NumberNode& node) override;
    void visit(const BinaryOpNode& node) override;
	void visit(const UnaryOpNode& node) override;
		
	
};