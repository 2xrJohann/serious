#include "./lexer/lexer.h"
#include <iostream>

std::string_view to_string(TokenKind kind) {
    switch (kind) {
        case TokenKind::Identifier:     return "Identifier";
        case TokenKind::IntLiteral:     return "IntLiteral";
        case TokenKind::FloatLiteral:   return "FloatLiteral";
        case TokenKind::StringLiteral:  return "StringLiteral";
        case TokenKind::BooleanLiteral: return "BooleanLiteral";

        case TokenKind::Const:          return "Const";
        case TokenKind::Let:            return "Let";

        case TokenKind::If:             return "If";
        case TokenKind::Else:           return "Else";
        case TokenKind::While:          return "While";
        case TokenKind::For:            return "For";
        case TokenKind::Return:         return "Return";

        case TokenKind::Plus:           return "Plus";
        case TokenKind::Minus:          return "Minus";
        case TokenKind::Star:           return "Star";
        case TokenKind::Slash:          return "Slash";
        case TokenKind::Percent:        return "Percent";

        case TokenKind::EqualEqual:     return "EqualEqual";
        case TokenKind::BangEqual:      return "BangEqual";
        case TokenKind::Less:           return "Less";
        case TokenKind::LessEqual:      return "LessEqual";
        case TokenKind::Greater:        return "Greater";
        case TokenKind::GreaterEqual:   return "GreaterEqual";
        case TokenKind::AndAnd:         return "AndAnd";
        case TokenKind::OrOr:           return "OrOr";
        case TokenKind::Bang:           return "Bang";

        case TokenKind::Assign:         return "Assign";
        case TokenKind::PlusEqual:      return "PlusEqual";
        case TokenKind::MinusEqual:     return "MinusEqual";
        case TokenKind::StarEqual:      return "StarEqual";
        case TokenKind::SlashEqual:     return "SlashEqual";
        case TokenKind::PipeGreater:    return "PipeGreater";
        case TokenKind::Arrow:          return "Arrow";

        case TokenKind::Dot:            return "Dot";
        case TokenKind::Comma:          return "Comma";
        case TokenKind::Colon:          return "Colon";
        case TokenKind::Semicolon:      return "Semicolon";

        case TokenKind::LParen:         return "LParen";
        case TokenKind::RParen:         return "RParen";
        case TokenKind::LBrace:         return "LBrace";
        case TokenKind::RBrace:         return "RBrace";
        case TokenKind::LBracket:       return "LBracket";
        case TokenKind::RBracket:       return "RBracket";

        case TokenKind::Eof:            return "Eof";
        default:                        return "Unknown";
    }
}

int main() {
    std::string src = R"(
        const x = 10;
        let y = x |> (v) => v + 5;
        if (y > 5) return y;
    )";

    Lexer lexer(src);
    auto tokens = lexer.lexAll();

    for (auto &t : tokens)
        std::cout << to_string(t.kind) << "  " << t.text << "\n";
}
