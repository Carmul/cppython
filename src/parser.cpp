#include "parser.h"
#include "Token.h"
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
		std::cerr << "Error: expected token type " << tokenTypeToString(type) << ", got " << tokenTypeToString(currentToken.type) << std::endl;
		throw 1;
	}
}

// return the root of the AST (program node)
ASTNodePtr Parser::parse() {
	return program();
}

// program : statements EOF_TOKEN
ASTNodePtr Parser::program() {
	std::vector<ASTNodePtr> stmts = statements();
	eat(TokenType::EOF_TOKEN);
	return std::make_unique<BlockNode>(std::move(stmts));
}

// statements : ( compound_statement | simple_statement NEWLINE )*
std::vector<ASTNodePtr> Parser::statements() {
	std::vector<ASTNodePtr> stmts;

	while (currentToken.type != TokenType::EOF_TOKEN && currentToken.type != TokenType::DEDENT) {
		
		if (currentToken.type == TokenType::IF || currentToken.type == TokenType::WHILE) { // add more compound statements here
			stmts.push_back(compound_stmt());
		}
		else {
			stmts.push_back(simple_stmt());
			if (currentToken.type == TokenType::EOF_TOKEN) {
				break;
			}
			eat(TokenType::NEWLINE);
		}
	}

	return stmts;
}

// simple_stmt : assignment_stmt | expr
ASTNodePtr Parser::simple_stmt() {
	if (currentToken.type == TokenType::NAME) {
		if (lexer.peekNextToken().type == TokenType::EQUAL)
			return assignment_stmt();
	}
	return expr();
}

// compound_statement : if_statement
ASTNodePtr Parser::compound_stmt() {
	if (currentToken.type == TokenType::WHILE)
		return while_stmt();
	else if (currentToken.type == TokenType::IF)
		return if_stmt();
}

ASTNodePtr Parser::while_stmt() {
	eat(TokenType::WHILE);
	auto condition = expr();
	eat(TokenType::COLON);
	eat(TokenType::NEWLINE);
	std::vector<ASTNodePtr> body = block();
	return std::make_unique<WhileNode>(std::move(condition), std::make_unique<BlockNode>(std::move(body)));
}

// if_statement : IF expr COLON NEWLINE block
ASTNodePtr Parser::if_stmt() {
	eat(TokenType::IF);
	auto condition = expr();
	eat(TokenType::COLON);
	eat(TokenType::NEWLINE);
	std::vector<ASTNodePtr> body = block();
	auto ifnode = std::make_unique<IfNode>(std::move(condition), std::make_unique<BlockNode>(std::move(body)), nullptr);
	// Handle optional elif and else
	if (currentToken.type == TokenType::ELIF) {
		auto elifNode = elif_stmt();
		ifnode->elseBody = std::move(elifNode);
	}
	else if (currentToken.type == TokenType::ELSE) {
		auto elseNode = else_stmt();
		ifnode->elseBody = std::move(elseNode);
	}
	return ifnode;
}

// elif_statement : ELIF expr COLON NEWLINE block
ASTNodePtr Parser::elif_stmt() {
	eat(TokenType::ELIF);
	auto condition = expr();
	eat(TokenType::COLON);
	eat(TokenType::NEWLINE);
	std::vector<ASTNodePtr> body = block();
	auto elifNode = std::make_unique<IfNode>(std::move(condition), std::make_unique<BlockNode>(std::move(body)), nullptr);
	// Handle optional elif and else
	if (currentToken.type == TokenType::ELIF) {
		auto nextElifNode = elif_stmt();
		elifNode->elseBody = std::move(nextElifNode);
	}
	else if (currentToken.type == TokenType::ELSE) {
		auto elseNode = else_stmt();
		elifNode->elseBody = std::move(elseNode);
	}
	return elifNode;
}

// else_statement : ELSE COLON NEWLINE block
ASTNodePtr Parser::else_stmt() {
	eat(TokenType::ELSE);
	eat(TokenType::COLON);
	eat(TokenType::NEWLINE);
	std::vector<ASTNodePtr> body = block();
	return std::make_unique<BlockNode>(std::move(body));
}

// block : INDENT statements DEDENT
std::vector<ASTNodePtr> Parser::block() {
	eat(TokenType::INDENT);
	auto stmts = statements();
	eat(TokenType::DEDENT);
	return stmts;
}

// assignment_stmt : NAME EQUAL expr
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


ASTNodePtr Parser::function_call(std::string func_name) {
	// Not implemented yet
	std::vector<ASTNodePtr> args;
	eat(TokenType::LPAR);
	if (currentToken.type != TokenType::RPAR) {
		// Parse arguments (not implemented yet)
		args.push_back(expr());
		while (currentToken.type == TokenType::COMMA) {
			eat(TokenType::COMMA);
			args.push_back(expr());
		}
	}
	eat(TokenType::RPAR);

	return std::make_unique<FunctionCallNode>(func_name, std::move(args));
}


// expr : disjunction
ASTNodePtr Parser::expr() {
	return disjunction();
}

// disjunction: conjunction ( OR conjunction )*
ASTNodePtr Parser::disjunction() {
	auto node = conjunction();
	if(currentToken.type == TokenType::OR) {
		std::string op = currentToken.value;
		eat(TokenType::OR);
		node = std::make_unique<BinaryOpNode>(std::move(node), op, disjunction());
	}
	return node;
}

// conjunction: inversion ( AND inversion )*
ASTNodePtr Parser::conjunction() {
	auto node = inversion();
	if(currentToken.type == TokenType::AND) {
		std::string op = currentToken.value;
		eat(TokenType::AND);
		node = std::make_unique<BinaryOpNode>(std::move(node), op, conjunction());
	}
	return node;
}

// inversion: not* comparison
ASTNodePtr Parser::inversion() {
	if (currentToken.type == TokenType::NOT) {
		std::string op = currentToken.value;
		eat(TokenType::NOT);
		return std::make_unique<UnaryOpNode>(op, inversion());
	}
	return comparison();
}

// comparison: arith_expr((EQEQUAL | NOTEQUAL | LESSEQUAL | GREATEREQUAL | LESS | GREATER) arith_expr)*
ASTNodePtr Parser::comparison() {
	auto node = arith_expr();
	while (currentToken.type == TokenType::LESS || currentToken.type == TokenType::LESSEQUAL ||
		currentToken.type == TokenType::GREATER || currentToken.type == TokenType::GREATEREQUAL ||
		currentToken.type == TokenType::EQEQUAL || currentToken.type == TokenType::NOTEQUAL) {

		std::string op = currentToken.value;
		if (currentToken.type == TokenType::LESS) {
			eat(TokenType::LESS);
		}
		else if (currentToken.type == TokenType::LESSEQUAL) {
			eat(TokenType::LESSEQUAL);
		}
		else if (currentToken.type == TokenType::GREATER) {
			eat(TokenType::GREATER);
		}
		else if (currentToken.type == TokenType::GREATEREQUAL) {
			eat(TokenType::GREATEREQUAL);
		}
		else if (currentToken.type == TokenType::EQEQUAL) {
			eat(TokenType::EQEQUAL);
		}
		else if (currentToken.type == TokenType::NOTEQUAL) {
			eat(TokenType::NOTEQUAL);
		}
		node = std::make_unique<BinaryOpNode>(std::move(node), op, arith_expr());
	}
	return node;
}

// arith_expr : term ( (PLUS | MINUS) term)*
ASTNodePtr Parser::arith_expr() {

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

// term : factor ( (MUL | DIV | PERCENT) factor)*
ASTNodePtr Parser::term() {
	ASTNodePtr node = factor();
	while (currentToken.type == TokenType::MUL || currentToken.type == TokenType::DIV || currentToken.type == TokenType::PERCENT) {
		std::string op = currentToken.value;
		if (currentToken.type == TokenType::MUL) {
			eat(TokenType::MUL);
		}
		else if (currentToken.type == TokenType::DIV) {
			eat(TokenType::DIV);
		}
		else if (currentToken.type == TokenType::PERCENT) {
			eat(TokenType::PERCENT);
		}
		node = std::make_unique<BinaryOpNode>(std::move(node), op, factor());
	}
	return node;

}

// factor : (PLUS | NIMUS)? factor | NUMBER | LPAR expr RPAR | NAME | BOOLEAN | STRING | function_call
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
		std::string name = currentToken.value;
		eat(TokenType::NAME);
		// Check for function call
		if (currentToken.type == TokenType::LPAR) {
			
			return function_call(name);
		}
		auto node = std::make_unique<VarNode>(name);
		return node;
	}
	if (currentToken.type == TokenType::LPAR) {
		eat(TokenType::LPAR);
		auto node = expr();
		eat(TokenType::RPAR);
		return node;
	}
	if (currentToken.type == TokenType::BOOLEAN) {
		auto node = std::make_unique<BooleanNode>(currentToken.value);
		eat(TokenType::BOOLEAN);
		return node;
	}
	if (currentToken.type == TokenType::STRING) {
		auto node = std::make_unique<StringNode>(currentToken.value);
		eat(TokenType::STRING);
		return node;
	}

	std::cerr << "Error: Invalide factor, got " << static_cast<int>(currentToken.type) << std::endl;
	throw 1;
}
