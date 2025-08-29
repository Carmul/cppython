#pragma once

#include "lexer.h"
#include "ast.h"


class Parser {

public:
	explicit Parser(Lexer& lexer);
	ASTNodePtr parse();

private:
	Lexer& lexer;
	Token currentToken;
	void eat(TokenType type);
	// Grammar rules
	ASTNodePtr  expr(); // expr : term ((PLUS | MINUS) term)*
	ASTNodePtr  term(); // term : factor ((MUL | DIV) factor)*
	ASTNodePtr  factor(); // factor : INTEGER | LPAREN expr RPAREN
	ASTNodePtr  program(); // program : (statement NEWLINE)* EOF
	ASTNodePtr statement(); // statement : print_statement | expr
	ASTNodePtr print_statement(); // print_statement : PRINT LPAR expr RPAR
};


