#include "parser.h"
#include <memory.h>
#include <iostream>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
	currentToken = lexer.getNextToken();
    // Skip initial newlines
    while (currentToken.type == TokenType::NEWLINE) {
        currentToken = lexer.getNextToken();
    }
}

void Parser::eat(TokenType type) {
	if (currentToken.type == type) {
		currentToken = lexer.getNextToken();
	}
	else {
		std::cerr << "Error: expected token type " << static_cast<int>(type) << ", got " << static_cast<int>(currentToken.type) << std::endl;
		throw 1;
	}
}

// return the root of the AST (program node)
ASTNodePtr Parser::parse() {
	return program();
}

// program : (statement NEWLINE)* EOF
ASTNodePtr Parser::program() {
	std::vector<ASTNodePtr> statements;

	while (currentToken.type != TokenType::EOF_TOKEN) {

		ASTNodePtr stmt = statement();
		if (stmt) {
			statements.push_back(std::move(stmt));
		}
		// If last line was not newline, break
		if (currentToken.type == TokenType::EOF_TOKEN) {
			break;
		}
		eat(TokenType::NEWLINE);
		
	}
	return std::make_unique<ProgramNode>(std::move(statements));
}

// statement : print_stmt | assignment_stmt | expr
ASTNodePtr Parser::statement() {
	if (currentToken.type == TokenType::PRINT) {
		return print_stmt();
	} 
	else if (currentToken.type == TokenType::NAME) {
		// Lookahead to check if next token is EQUAL, if so, it's an assignment, else it's an expr
		if (lexer.peekNextToken().type == TokenType::EQUAL)
			return assignment_stmt();
	}
	return expr();
}

ASTNodePtr Parser::assignment_stmt() {
	if (currentToken.type == TokenType::NAME) {
		std::string varName = currentToken.value;
		eat(TokenType::NAME);
		eat(TokenType::EQUAL);
		// ASTNodePtr valueNode
		// For now, we do not implement variable storage, return value of expr
		return std::make_unique<AssignmentNode>(std::make_unique<VarNode>(varName), expr());
	}
	std::cerr << "Error: Invalid assignment statement, expected IDENTIFIER, got " << static_cast<int>(currentToken.type) << std::endl;
	throw 1;
}

// print_stmt : PRINT LPAR expr RPAR
ASTNodePtr Parser::print_stmt() {
	eat(TokenType::PRINT);
	eat(TokenType::LPAR);
	auto node = expr();
	eat(TokenType::RPAR);
	return std::make_unique<PrintNode>(std::move(node));
}

// expr : term ( (PLUS | MINUS) term)*
ASTNodePtr Parser::expr() {

	auto node = term();
	while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
		std::string op = currentToken.value;
		if (currentToken.type == TokenType::PLUS) {
			eat(TokenType::PLUS);
		}
		else if (currentToken.type == TokenType::MINUS) {
			eat(TokenType::MINUS);
		}
		node = std::make_unique<BinaryOpNode>(std::move(node), op, term());
	}
	return node;

}

// term : factor ( (MUL | DIV) factor)*
ASTNodePtr Parser::term() {
	ASTNodePtr node = factor();
	while (currentToken.type == TokenType::MUL || currentToken.type == TokenType::DIV) {
		std::string op = currentToken.value;
		if (currentToken.type == TokenType::MUL) {
			eat(TokenType::MUL);
		}
		else if (currentToken.type == TokenType::DIV) {
			eat(TokenType::DIV);
		}
		node = std::make_unique<BinaryOpNode>(std::move(node), op, factor());
	}
	return node;

}

// factor : (PLUS | NIMUS) factor | NUMBER | LPAR expr RPAR | NAME
ASTNodePtr Parser::factor() {
	if (currentToken.type == TokenType::PLUS) {
		std::string op = currentToken.value;
		eat(TokenType::PLUS);
		return std::make_unique<UnaryOpNode>(op, factor());
	}
	if (currentToken.type == TokenType::MINUS) {
		std::string op = currentToken.value;
		eat(TokenType::MINUS);
		return std::make_unique<UnaryOpNode>(op, factor());
	}
	if (currentToken.type == TokenType::NUMBER) {
		auto node = std::make_unique<NumberNode>(currentToken.value);
		eat(TokenType::NUMBER);
		return node;
	}
	if (currentToken.type == TokenType::NAME) {
		auto node = std::make_unique<VarNode>(currentToken.value);
		eat(TokenType::NAME);
		return node;
	}
	if (currentToken.type == TokenType::LPAR) {
		eat(TokenType::LPAR);
		auto node = expr();
		eat(TokenType::RPAR);
		return node;
	}
	std::cerr << "Error: Invalide factor, got " << static_cast<int>(currentToken.type) << std::endl;
	throw 1;
}
