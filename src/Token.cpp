#include <string>
#include "Token.h"

std::string tokenTypeToString(TokenType type) {
	switch (type) {
		case TokenType::INTEGER: return "INTEGER";
		case TokenType::PLUS:    return "PLUS";
		case TokenType::MINUS:   return "MINUS";
		case TokenType::MUL:     return "MUL";
		case TokenType::DIV:     return "DIV";
		case TokenType::NEWLINE:     return "NEWLINE";
		case TokenType::INDENT:     return "INDENT";
		case TokenType::DEDENT:     return "DEDENT";
		case TokenType::LPAR:  return "(";
		case TokenType::RPAR:  return ")";
		case TokenType::EOF_TOKEN: return "EOF";
		default:                 return "UNKNOWN";
	}
}

std::string Token::toString() const {
	std::string str = "Token(" + tokenTypeToString(type) + ", \"" + value + "\")";
	return str;
}
