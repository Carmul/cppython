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
		case TokenType::NAME:  return "NAME";
		case TokenType::EQUAL: return "EQUAL";
		case TokenType::STRING:return "STRING";
		case TokenType::BOOLEAN:return "BOOLEAN";
		case TokenType::EQEQUAL: return "EQEQUAL";
		case TokenType::NOTEQUAL: return "NOTEQUAL";
		case TokenType::LESSEQUAL: return "LESSEQUAL";
		case TokenType::GREATEREQUAL: return "GREATEREQUAL";
		case TokenType::LESS:  return "LESS";
		case TokenType::GREATER: return "GREATER";
		case TokenType::COLON: return "COLON";
		case TokenType::IF:    return "IF";
		case TokenType::ELSE:  return "ELSE";
		case TokenType::ELIF:  return "ELIF";
		case TokenType::WHILE: return "WHILE";
		case TokenType::COMMA: return "COMMA";
		case TokenType::OR: return "OR";
		case TokenType::AND: return "AND";
		case TokenType::NOT: return "NOT";
		case TokenType::EOF_TOKEN: return "EOF";
		default:                 return "UNKNOWN";
	}
}

std::string Token::toString() const {
	std::string str = "Token(" + tokenTypeToString(type) + ", \"" + value + "\")";
	return str;
}
