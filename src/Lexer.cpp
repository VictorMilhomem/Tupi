#include "Lexer.h"


std::vector<Lexer::Token> Lexer::Lexer::tokenize() {
    std::vector<Token> tokens;
    std::smatch match;
    LexerError error;
    while (!this->m_at_eof()) {
        char current = this->m_source[this->at()];
        if (current == '\n') {
            this->m_advance_line();
            continue;
        }

        if (isspace(current)) {
            this->m_advanceN(1);
            continue;
        }

        switch (current) {
            case '(':
                tokens.push_back({TokenKind::LPAREN, this->m_source});
                this->m_advanceN(1);
                continue;
            case ')':
                tokens.push_back({TokenKind::RPAREN, this->m_source});
                this->m_advanceN(1);
                continue;
            case '[':
                tokens.push_back({TokenKind::LBRACKET, this->m_source});
                this->m_advanceN(1);
                continue;
            case ']':
                tokens.push_back({TokenKind::RBRACKET, this->m_source});
                this->m_advanceN(1);
                continue;
            case '{':
                tokens.push_back({TokenKind::LBRACE, this->m_source});
                this->m_advanceN(1);
                continue;
            case '}':
                tokens.push_back({TokenKind::RBRACE, this->m_source});
                this->m_advanceN(1);
                continue;
            case '<':
                if (this->m_lookahead(1) == '=') {
                    tokens.push_back({TokenKind::LTEQUAL, this->m_source});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::LT, this->m_source});
                    this->m_advanceN(1);
                }
                continue;
            case '>':
                if (this->m_lookahead(1) == '=') {
                    tokens.push_back({TokenKind::GTEQUAL, this->m_source});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::GT, this->m_source});
                    this->m_advanceN(1);
                }
                continue;
            case ':':
                tokens.push_back({TokenKind::COLON, this->m_source});
                this->m_advanceN(1);
                continue;
            case ';':
                tokens.push_back({TokenKind::SEMICOLON, this->m_source});
                this->m_advanceN(1);
                continue;
            case '=':
                if (this->m_lookahead(1) == '=') {
                    tokens.push_back({TokenKind::EQUAL, this->m_source});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::ASSIGN, this->m_source});
                    this->m_advanceN(1);
                }
                continue;
            case '+':
                if (this->m_lookahead(1) == '+') {
                    tokens.push_back({TokenKind::INCREMENT, this->m_source});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::PLUS, this->m_source});
                    this->m_advanceN(1);
                }
                continue;
            case '-':
                if (this->m_lookahead(1) == '-') {
                    tokens.push_back({TokenKind::INCREMENT, this->m_source});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::MINUS, this->m_source});
                    this->m_advanceN(1);
                }
                continue;
            case '*':
                tokens.push_back({TokenKind::MULT, this->m_source});
                this->m_advanceN(1);
                continue;
            case '/':
                tokens.push_back({TokenKind::DIV, this->m_source});
                this->m_advanceN(1);
                continue;
            case '~':
                tokens.push_back({TokenKind::BITWISE, this->m_source});
                this->m_advanceN(1);
                continue;
            case '!':
                tokens.push_back({TokenKind::NOT, this->m_source});
                this->m_advanceN(1);
                continue;
            default:
                // check the regex patterns
                std::string remaining = this->m_source.substr(this->at());
                bool matched = false;
                for (const auto& [_, pattern_pair] : m_patterns) {
                    if (m_tryMatch(remaining, match, pattern_pair.first, pattern_pair.second, tokens)) {
                        matched = true;
                        break;  // A token was matched, move on to the next portion of the input
                    }
                }
                if (!matched) {
                    this->m_had_error = true;
                    error = LexerError{current + remaining, this->m_location, "Unknown Token"};
                    this->m_advanceN(1);
                }

                continue;
        }
    }

    if (this->m_had_error) m_raise_error(error);

    return tokens;
}