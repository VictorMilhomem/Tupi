#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>

namespace Lexer {
#define LEXER_ERROR (-1)

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
        size_t line;
        size_t column;
    };

    struct LexerError {
        std::string c;
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
        std::string m_get_token_representation(TokenKind token_kind);
        static void m_raise_error(const LexerError& error) {
            std::cerr << "Unknown Token "<< error.c <<" at Line:" <<
                error.position.line << "\nColumn:" << error.position.column;
            exit(LEXER_ERROR);
        };
        void m_advanceN(const int n) {
            this->m_location.column += n;
        };

        void m_advance_line() {
            this->m_advanceN(1);
            this->m_location.line++;
        };

        void m_push_token(const Token& token) {
            this->m_tokens.push_back(token);
        };

        bool m_at_eof() const {
            return this->m_location.column >= this->m_source.size();
        }


    public:
        Lexer(const std::string& source) {
            this->m_source = source;
            this->m_location = {1,0};
            this->m_had_error = false;
        };
        ~Lexer() = default;

        void tokenize();
        std::vector<Token> get_tokens() {return this->m_tokens; };




    };
}

#endif //LEXER_H
