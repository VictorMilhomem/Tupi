#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <unordered_map>
#include <vector>
#include <bits/regex.h>

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
    };

    struct LexerLocation {
        int line;
        int column;
    };

    struct LexerError {
        LexerLocation position;
        std::string error_message;
    };

    class Lexer {
    private:
        std::string m_source;
        LexerLocation m_location;
        std::vector<Token> m_tokens;
        bool m_had_error;
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

        const std::vector<std::regex> m_patterns; // TODO: CREATE THE PATTERNS FOR THE LEXER

        void advanceN(const int n) {
            this->m_location.column += n;
        };

        void advance_line() {
            this->advanceN(1);
            this->m_location.line++;
        };

        void push_token(Token token) {
            this->m_tokens.push_back(std::move(token));
        };

        bool at_eof() const {
            return this->m_location.column >= this->m_source.size();
        }


    public:
        Lexer(std::string source) {
            this->m_source = std::move(source);
            this->m_location = {1,0};
            this->m_had_error = false;
        };

        std::vector<Token> tokenize();




    };
}

#endif //LEXER_H
