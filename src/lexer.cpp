#include "lexer.h"
#include <cctype>

#include <iostream>

// TODO: add current line and col for better errors
Lexer::Lexer(const std::string& text) : text(text), pos(0), lastWasNewline(false), pendingDedents(0) {
	currentChar = text.empty() ? '\0' : text[pos];
}

void Lexer::advance() {
	pos++;
	currentChar = (pos < text.size()) ? text[pos] : '\0';
}

void Lexer::skipWhitespace() {
	while (currentChar != '\0' && currentChar == ' ')
		advance();
}

std::string Lexer::number() {
	// TODO: handle 123abc cases and maybe hex, binary, etc.
	std::string result;
	while (currentChar != '\0' && isdigit(currentChar)) {
		result += currentChar;
		advance();
		if (currentChar == '.') {
			result += currentChar;
			advance();
			break;
		}
	}
	while (currentChar != '\0' && isdigit(currentChar)) {
		result += currentChar;
		advance();
	}
	if (result.back() == '.') // 123. => 123.0
		result += '0';
	return result;
}

std::string Lexer::identifier() {
	std::string result;
	while (currentChar != '\0' && (isalnum(currentChar) || currentChar == '_')) {
		result += currentChar;
		advance();
	}
	return result;
}

Token Lexer::getNextToken() {

	while (currentChar != '\0') {

		// Handle queued dedents
		if (pendingDedents > 0) {
			pendingDedents--;
			return { TokenType::DEDENT, "" };
		}
		// Handles indentation
		if (lastWasNewline) { 
			lastWasNewline = false;
			// Count spaces at start of new line
			int spaceCount = 0;
			while (currentChar == ' ') {
				spaceCount++;
				advance();
			}
			// ignore multiple consecutive newlines
			if (currentChar == '\n') {
				advance(); 
				lastWasNewline = true;
				continue;
			}
			
			if (spaceCount % 4 != 0) {
				std::cerr << "IndentationError: unexpected indent" << std::endl;
				throw 1;
			}
			 
			if (spaceCount / 4 > indentStack.back()) {
				indentStack.push_back(spaceCount / 4);
				return { TokenType::INDENT, "" };
			} 
			else if (spaceCount / 4 < indentStack.back()) {
				while (!indentStack.empty() && spaceCount / 4 < indentStack.back()) {
					indentStack.pop_back();
					pendingDedents++;
				}
				if (pendingDedents > 0) {
					pendingDedents--;
					return { TokenType::DEDENT, "" };
				}
			}
		}

		if (currentChar == '\n') { advance(); lastWasNewline = true; return { TokenType::NEWLINE, "" }; }

		if (isspace(currentChar)) {
			skipWhitespace();
			continue;
		}

		// TODO: add single quote strings and escape sequences 
		if (currentChar == '\"') {
			advance(); // skip opening quote
			std::string strVal;
			while (currentChar != '\0' && currentChar != '\"') {
				strVal += currentChar;
				advance();
			}
			if (currentChar == '\"') {
				advance(); // skip closing quote
				return { TokenType::STRING, strVal };
			} else {
				std::cerr << "Error: Unterminated string literal" << std::endl;
				throw 1;
			}
		}

		// Identifiers and keywords
		if (std::isalpha(currentChar) || currentChar == '_') {
			std::string id = identifier();
			// Check for keywords
			if (id == "print") return { TokenType::PRINT, id };
			if (id == "True") return { TokenType::BOOLEAN, id };
			if (id == "False") return { TokenType::BOOLEAN, id };
			if (id == "if") return { TokenType::IF, id };
			if (id == "else") return { TokenType::ELSE, id };
			if (id == "elif") return { TokenType::ELIF, id };
			// Return Name
			return { TokenType::NAME, id };
		}

		if (isdigit(currentChar)) return { TokenType::NUMBER, number() };

		if (currentChar == '+') { advance(); return { TokenType::PLUS, "+" }; }
		if (currentChar == '-') { advance(); return { TokenType::MINUS, "-" }; }
		if (currentChar == '*') { advance(); return { TokenType::MUL, "*" }; }
		if (currentChar == '/') { advance(); return { TokenType::DIV, "/" }; }
		if (currentChar == '(') { advance(); return { TokenType::LPAR, "(" }; }
		if (currentChar == ')') { advance(); return { TokenType::RPAR, ")" }; }
		if (currentChar == ':') { advance(); return { TokenType::COLON, ":" }; }


		if (currentChar == '=') {
			advance();
			if (currentChar == '=') { advance(); return { TokenType::EQEQUAL, "==" }; }
			return { TokenType::EQUAL, "=" };
		}
		if (currentChar == '<') {
			advance();
			if (currentChar == '=') { advance(); return { TokenType::LESSEQUAL, "<=" }; }
			return { TokenType::LESS, "<" };
		}
		if (currentChar == '>') {
			advance();
			if (currentChar == '=') { advance(); return { TokenType::GREATEREQUAL, ">=" }; }
			return { TokenType::GREATER, ">" };
		}
		if (currentChar == '!') {
			advance();
			if (currentChar == '=') { advance(); return { TokenType::NOTEQUAL, "!=" }; }
			std::cerr << "Error: Unexpected character '!'" << std::endl;
			throw 1;
		}
		// Add more token types as needed
		std::cerr << "Error: Unexpected character '" << currentChar << "'" << std::endl;
		throw 1;
	}
	// Handle any remaining dedents at EOF
	if (indentStack.size() > 1) {
		indentStack.pop_back();
		return { TokenType::DEDENT, "" };
	}
	return { TokenType::EOF_TOKEN, "" };
}

Token Lexer::peekNextToken() {
	// Save state
	size_t savedPos = pos;
	char savedChar = currentChar;
	std::vector<int> savedIndentStack = indentStack;
	int savedPendingDedents = pendingDedents;
	bool savedLastWasNewline = lastWasNewline;
	Token nextToken = getNextToken();
	// Restore state
	pos = savedPos;
	currentChar = savedChar;
	indentStack = savedIndentStack;
	pendingDedents = savedPendingDedents;
	lastWasNewline = savedLastWasNewline;
	return nextToken;
}