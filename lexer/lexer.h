#pragma once
#include <string_view>
#include <vector>
#include "token.h"

class Lexer {
public:
    explicit Lexer(std::string_view src);
    std::vector<Token> lexAll();

private:
    std::string_view source;
    const char* start;
    const char* current;
    int line;
    int column;
    int startColumn; // pos tracking

    // utils
    bool isAtEnd() const;
    char advance();
    bool match(char expected);
    char peek() const;
    char peekNext() const;
    void skipWhitespace();

    void startToken(); // pos tracking

    Token makeToken(TokenKind kind) const;
    Token identifier();
    Token number();
    Token string();
};
