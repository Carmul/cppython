#pragma once

enum class TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    NEWLINE,
    LPAR,
    RPAR,
    INDENT,
    DEDENT,
    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string value;

    std::string toString() const;
};