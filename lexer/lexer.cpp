#include "lexer.h"
#include "token.h"
#include <cctype>
#include <unordered_map>

Lexer::Lexer(std::string_view src)
    : source(src),
      start(src.data()),
      current(src.data()),
      line(1),
      column(1),
      startColumn(1) {}

Token Lexer::identifier() {
    startToken();

    while (std::isalnum(peek()) || peek() == '_') {
        advance();
    }

    std::string_view text(start, current - start);

    static const std::unordered_map<std::string_view, TokenKind> keywords = {
        {"const", TokenKind::Const},
        {"let", TokenKind::Let},
        {"if", TokenKind::If},
        {"else", TokenKind::Else},
        {"while", TokenKind::While},
        {"for", TokenKind::For},
        {"return", TokenKind::Return},
        {"true", TokenKind::BooleanLiteral},
        {"false", TokenKind::BooleanLiteral},
    };

    auto it = keywords.find(text);
    TokenKind kind = (it != keywords.end()) ? it->second : TokenKind::Identifier;

    return Token{kind, text, line, startColumn};
}

Token Lexer::number() {
    startToken();

    while (std::isdigit(peek())) {
        advance();
    }

    bool isFloat = false;

    if (peek() == '.' && std::isdigit(peekNext())) {
        isFloat = true;
        advance();

        while (std::isdigit(peek())) {
            advance();
        }
    }

    std::string_view text(start, current - start);

    return Token{
        isFloat ? TokenKind::FloatLiteral : TokenKind::IntLiteral,
        text,
        line,
        startColumn
    };
}

Token Lexer::string() {
    startToken();

    char quote = advance();

    while (!isAtEnd()) {
        char c = peek();

        if (c == quote) {
            advance();
            break;
        }

        if (c == '\\') {
            advance();
            if (!isAtEnd()) advance();
            continue;
        }

        advance();

        if (c == '\n') {
            line++;
            column = 1;
        }
    }

    std::string_view text(start + 1, current - start - 2);

    return Token{TokenKind::StringLiteral, text, line, startColumn};
}

Token Lexer::makeToken(TokenKind kind) const {
    return { kind, std::string_view(start, current - start), line, startColumn };
}

void Lexer::startToken() {
    start = current;
    startColumn = column;
}

bool Lexer::isAtEnd() const {
    return current >= source.data() + source.size();
}

char Lexer::advance() {
    char c = *current++;
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    
    return c;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return *current;
}


char Lexer::peekNext() const {
    if (current + 1 >= source.data() + source.size()) return '\0';
    return *(current + 1);
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            case '\n':
                advance();
                break;
            default:
                return;
        }
    }
}
