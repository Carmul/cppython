#pragma once

#include <string>
#include <vector> 
#include "Token.h"

class Lexer {
public:
    explicit Lexer(const std::string& text);
    Token getNextToken();
    
private:
    std::string text;
    size_t pos;
    char currentChar;
    std::vector<int> indentStack = { 0 };
    int pendingDedents;
    bool lastWasNewline;
    void advance();
    void skipWhitespace();
    std::string number();
};
