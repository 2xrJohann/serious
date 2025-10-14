#pragma once
#include <string_view>
#include <string>

enum class TokenKind {
    Identifier,
    IntLiteral,
    FloatLiteral,
    StringLiteral,
    BooleanLiteral,

    Const, Let,

    If, Else, While, For, Return,

    Plus, Minus, Star, Slash, Percent,
    EqualEqual, BangEqual,
    Less, LessEqual, Greater, GreaterEqual,
    AndAnd, OrOr, Bang,

    Assign, PlusEqual, MinusEqual, StarEqual, SlashEqual,
    PipeGreater, // |>
    Arrow,       // =>
    Dot, Comma, Colon, Semicolon,
    LParen, RParen, LBrace, RBrace, LBracket, RBracket,

    Eof
};

struct Token {
    TokenKind kind;
    std::string_view text;
    int line;
    int column;
};