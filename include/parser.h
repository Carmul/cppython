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
	ASTNodePtr  expr();					// expr : comparison ( (OR | AND) comparison )*
	ASTNodePtr  disjunction();			// disjunction : conjunction ( OR conjunction )*
	ASTNodePtr  conjunction();			// conjunction : inversion ( AND inversion )*
	ASTNodePtr  inversion();			// inversion : NOT inversion | comparison
	ASTNodePtr  comparison();			// comparison : arith_expr ( (EQEQUAL | NOTEQUAL | LESSEQUAL | GREATEREQUAL | LESS | GREATER) arith_expr )*
	ASTNodePtr arith_expr();			// term ((PLUS | MINUS) term)*
	ASTNodePtr  term();					// term : factor ((MUL | DIV) factor)*
	ASTNodePtr  factor();				// factor : INTEGER | LPAREN expr RPAREN | (PLUS | MINUS) factor | BOOLEAN | STRING | NAME | function_call

	ASTNodePtr  program();				// program : statements EOF_TOKEN
	std::vector<ASTNodePtr> statements();// ( compound_statement | simple_statement NEWLINE )*
	ASTNodePtr simple_stmt();		// simple_statement : print_stmt | assignment_stmt | expr
	ASTNodePtr compound_stmt();	// compound_statement : if_statement
	ASTNodePtr if_stmt();		// if_statement : IF expr COLON NEWLINE block ( elif_stmt | else )?
	ASTNodePtr elif_stmt();		// elif_statement : ELIF expr COLON NEWLINE block ( elif_stmt | else )?
	ASTNodePtr else_stmt();		// else_statement : ELSE COLON NEWLINE block
	ASTNodePtr while_stmt();	// while_statement : WHILE expr COLON NEWLINE block
	std::vector<ASTNodePtr> block();					// block : INDENT statements DEDENT
	ASTNodePtr assignment_stmt();		// assignment_stmt : IDENTIFIER ASSIGN expr
	ASTNodePtr function_call(std::string func_name);		// function_call : NAME LPAR (expr (COMMA expr)*)? RPAR
};


