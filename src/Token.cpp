#include <string>
#include "Token.h"

std::string tokenTypeToString(TokenType type) {
	switch (type) {
		case TokenType::NUMBER: return "NUMBER";
		case TokenType::PLUS:    return "PLUS";
		case TokenType::MINUS:   return "MINUS";
		case TokenType::MUL:     return "MUL";
		case TokenType::DIV:     return "DIV";
		case TokenType::NEWLINE:     return "NEWLINE";
		case TokenType::INDENT:     return "INDENT";
		case TokenType::DEDENT:     return "DEDENT";
		case TokenType::LPAR:  return "LPAR";
		case TokenType::RPAR:  return "RPAR";
		case TokenType::PRINT: return "PRINT";
		case TokenType::NAME:  return "NAME";
		case TokenType::EQUAL: return "EQUAL";
		case TokenType::STRING:return "STRING";
		case TokenType::BOOLEAN:return "BOOLEAN";
		case TokenType::EOF_TOKEN: return "EOF";
		default:                 return "UNKNOWN";
	}
}

std::string Token::toString() const {
	std::string str = "Token(" + tokenTypeToString(type) + ", \"" + value + "\")";
	return str;
}
