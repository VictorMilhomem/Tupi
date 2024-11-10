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
        ASSIGN,
        // BINARY OPERATORS
        EQUAL,
        GT,
        GTEQUAL,
        LT,
        LTEQUAL,

        // UNARY OPERATORS
        MINUS,
        PLUS,
        DIV,
        MULT,
        INCREMENT,
        DECREMENT,
        BITWISE,
        NOT,
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
        FOR,
        AND,
        OR
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
            {"and", TokenKind::AND},
            {"or", TokenKind::OR},
        };

        const std::unordered_map<std::string, std::pair<std::regex, TokenKind>> m_patterns = {
            {"int", {std::regex(R"(\d+)"), TokenKind::INTEGER}},
            {"float", {std::regex(R"(\d+\.\d+)"), TokenKind::FLOAT}},
            {"id", {std::regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"), TokenKind::IDENTIFIER}},
        };

        std::string m_get_token_representation(TokenKind token_kind);
        static void m_raise_error(const LexerError& error) {
            std::cerr << error.error_message << error.c <<" at Line:" <<
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

        size_t at() const {
            return this->m_location.column;
        }

        char m_lookahead(const int n) const {
            return this->m_source[this->m_location.column + n];
        }

        bool m_tryMatch(const std::string& remaining_input, std::smatch& match,
                  const std::regex& pattern, TokenKind type, std::vector<Token>& tokens)
        {
            if (std::regex_search(remaining_input, match, pattern) && match.position() == 0) {
                std::string token_value = match.str();
                if (type == TokenKind::IDENTIFIER) {
                    auto keyword_it = m_keywords.find(token_value);
                    if (keyword_it != m_keywords.end()) {
                        tokens.push_back({keyword_it->second, token_value});
                    } else {
                        tokens.push_back({TokenKind::IDENTIFIER, token_value});
                    }
                } else {
                    tokens.push_back({type, token_value});
                }

                m_advanceN(static_cast<int>(match.length()));
                return true;
            }
            return false;
        }
    public:
        Lexer(const std::string& source) {
            this->m_source = source;
            this->m_location = {1,0};
            this->m_had_error = false;
        };
        ~Lexer() = default;

        std::vector<Token> tokenize();
        std::vector<Token> get_tokens() {return this->m_tokens; };




    };
}

#endif //LEXER_H
