#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <unordered_map>

namespace Lexer {

    enum class TokenKind {
        TEOF,
        IDENTIFIER,
        INTEGER,
        FLOAT,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        SEMICOLON,
        COMMA,
        COLON,

        // BINARY OPERATORS
        EQUAL,
        GT,
        GTEQUAL,
        LT,
        LTEQUAL,

        // UNARY OPERATORS
        MINUS,
        PLUS,
        INCREMENT,
        DECREMENT,
        BITWISE,

        // KEYWORDS
        VOID,
        RETURN,
        KINT,
        KFLOAT,
        CONST,
        VAR,
        FN,
        IF,
        ELSE,
        ELIF,
        FOR
    };

    struct Token {
        TokenKind kind;
        std::string text;
        int position;
    };


    class Lexer {
    private:
        const std::unordered_map<std::string, TokenKind> m_keywords = {
            {"void", TokenKind::VOID},
            {"return", TokenKind::RETURN},
            {"int", TokenKind::KINT},
            {"float", TokenKind::KFLOAT},
            {"const", TokenKind::CONST},
            {"var", TokenKind::VAR},
            {"fn", TokenKind::FN},
            {"if", TokenKind::IF},
            {"elif", TokenKind::ELIF},
            {"else", TokenKind::ELSE},
            {"for", TokenKind::FOR},
        };
    };
}

#endif //LEXER_H
