#include "lexer.h"
#include "token.h"
#include <cctype>
#include <unordered_map>
#include <iostream>

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

    // Updated keyword map with switch-related keywords
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
        {"switch", TokenKind::Switch},
        {"case", TokenKind::Case},
        {"default", TokenKind::Default},
        {"break", TokenKind::Break},
        {"continue", TokenKind::Continue},
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

std::vector<Token> Lexer::lexAll() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();
        startToken();

        if (isAtEnd()) break;

        char c = advance();

        if (std::isalpha(c) || c == '_') {
            current--;
            tokens.push_back(identifier());
            continue;
        }

        if (std::isdigit(c)) {
            current--;
            tokens.push_back(number());
            continue;
        }

        if (c == '"' || c == '\'') {
            current--;
            tokens.push_back(string());
            continue;
        }

        switch (c) {
            case '+':
                tokens.push_back(makeToken(match('=') ? TokenKind::PlusEqual : TokenKind::Plus));
                break;
            case '-':
                if (match('=')) tokens.push_back(makeToken(TokenKind::MinusEqual));
                else if (match('>')) tokens.push_back(makeToken(TokenKind::Arrow));
                else tokens.push_back(makeToken(TokenKind::Minus));
                break;
            case '*':
                tokens.push_back(makeToken(match('=') ? TokenKind::StarEqual : TokenKind::Star));
                break;
            case '/':
                tokens.push_back(makeToken(match('=') ? TokenKind::SlashEqual : TokenKind::Slash));
                break;
            case '%':
                tokens.push_back(makeToken(TokenKind::Percent));
                break;
            case '=':
                tokens.push_back(makeToken(match('=') ? TokenKind::EqualEqual : TokenKind::Assign));
                break;
            case '!':
                tokens.push_back(makeToken(match('=') ? TokenKind::BangEqual : TokenKind::Bang));
                break;
            case '<':
                tokens.push_back(makeToken(match('=') ? TokenKind::LessEqual : TokenKind::Less));
                break;
            case '>':
                tokens.push_back(makeToken(match('=') ? TokenKind::GreaterEqual : TokenKind::Greater));
                break;
            case '&':
                if (match('&')) tokens.push_back(makeToken(TokenKind::AndAnd));
                break;
            case '|':
                if (match('|')) tokens.push_back(makeToken(TokenKind::OrOr));
                else if (match('>')) tokens.push_back(makeToken(TokenKind::PipeGreater));
                break;
            case '.': tokens.push_back(makeToken(TokenKind::Dot)); break;
            case ',': tokens.push_back(makeToken(TokenKind::Comma)); break;
            case ':': tokens.push_back(makeToken(TokenKind::Colon)); break;
            case ';': tokens.push_back(makeToken(TokenKind::Semicolon)); break;
            case '(': tokens.push_back(makeToken(TokenKind::LParen)); break;
            case ')': tokens.push_back(makeToken(TokenKind::RParen)); break;
            case '{': tokens.push_back(makeToken(TokenKind::LBrace)); break;
            case '}': tokens.push_back(makeToken(TokenKind::RBrace)); break;
            case '[': tokens.push_back(makeToken(TokenKind::LBracket)); break;
            case ']': tokens.push_back(makeToken(TokenKind::RBracket)); break;
            default:
                std::cerr << "Unknown char at line:" << line << " column:" << column;
                break;
        }
    }

    tokens.push_back(Token{TokenKind::Eof, "", line, column});
    return tokens;
}

bool Lexer::match(char expected) {
    auto peekchar = peek();
    if (peekchar == '\0') {
        return false;
    }

    if (peekchar != expected) {
        return false;
    }

    advance();
    return true;
}
