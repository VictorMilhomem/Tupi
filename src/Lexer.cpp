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
                tokens.push_back({TokenKind::LPAREN, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case ')':
                tokens.push_back({TokenKind::RPAREN, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case '[':
                tokens.push_back({TokenKind::LBRACKET, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case ']':
                tokens.push_back({TokenKind::RBRACKET, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case '{':
                tokens.push_back({TokenKind::LBRACE, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case '}':
                tokens.push_back({TokenKind::RBRACE, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case '<':
                if (this->m_lookahead(1) == '=') {
                    tokens.push_back({TokenKind::LTEQUAL, std::string(1,current)});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::LT, std::string(1,current)});
                    this->m_advanceN(1);
                }
                continue;
            case '>':
                if (this->m_lookahead(1) == '=') {
                    tokens.push_back({TokenKind::GTEQUAL, std::string(1,current)});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::GT, std::string(1,current)});
                    this->m_advanceN(1);
                }
                continue;
            case ':':
                tokens.push_back({TokenKind::COLON, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case ';':
                tokens.push_back({TokenKind::SEMICOLON, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case '=':
                if (this->m_lookahead(1) == '=') {
                    tokens.push_back({TokenKind::EQUAL, std::string(1,current)});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::ASSIGN, std::string(1,current)});
                    this->m_advanceN(1);
                }
                continue;
            case '+':
                if (this->m_lookahead(1) == '+') {
                    tokens.push_back({TokenKind::INCREMENT, std::string(1,current)});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::PLUS, std::string(1,current)});
                    this->m_advanceN(1);
                }
                continue;
            case '-':
                if (this->m_lookahead(1) == '-') {
                    tokens.push_back({TokenKind::INCREMENT, std::string(1,current)});
                    this->m_advanceN(2);
                }else {
                    tokens.push_back({TokenKind::MINUS, std::string(1,current)});
                    this->m_advanceN(1);
                }
                continue;
            case '*':
                tokens.push_back({TokenKind::MULT, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case '/':
                tokens.push_back({TokenKind::DIV, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case '~':
                tokens.push_back({TokenKind::BITWISE, std::string(1,current)});
                this->m_advanceN(1);
                continue;
            case '!':
                tokens.push_back({TokenKind::NOT, std::string(1,current)});
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

void Lexer::Lexer::m_debug_print_tokens(const Token& token) {
    const std::string text = token.text;
    switch (token.kind) {
        case TokenKind::TEOF:
            std::cout << "(Token: EOF, Content: '" << text << "')"<< std::endl;
            break;
        case TokenKind::IDENTIFIER:
            std::cout << "(Token: ID, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::INTEGER:
            std::cout << "(Token: INTEGER, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::FLOAT:
            std::cout << "(Token: FLOAT, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::LPAREN:
            std::cout << "(Token: LPAREN, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::RPAREN:
            std::cout << "(Token: RPAREN, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::LBRACE:
            std::cout << "(Token: LBRACE, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::RBRACE:
            std::cout << "(Token: RBRACE, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::LBRACKET:
            std::cout << "(Token: LBRACKET, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::RBRACKET:
            std::cout << "(Token: RBRACKET, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::SEMICOLON:
            std::cout << "(Token: SEMICOLON, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::COMMA:
            std::cout << "(Token: COMMA, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::COLON:
            std::cout << "(Token: COLON, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::ASSIGN:
            std::cout << "(Token: ASSIGN, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::EQUAL:
            std::cout << "(Token: EQUAL, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::GT:
            std::cout << "(Token: GT, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::GTEQUAL:
            std::cout << "(Token: GTEQUAL, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::LT:
            std::cout << "(Token: LT, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::LTEQUAL:
            std::cout << "(Token: LTEQUAL, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::MINUS:
            std::cout << "(Token: MINUS, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::PLUS:
            std::cout << "(Token: PLUS, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::DIV:
            std::cout << "(Token: DIV, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::MULT:
            std::cout << "(Token: MULT, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::INCREMENT:
            std::cout << "(Token: INCREMENT, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::DECREMENT:
            std::cout << "(Token: DECREMENT, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::BITWISE:
            std::cout << "(Token: BITWISE, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::NOT:
            std::cout << "(Token: NOT, Content: '" << text << "')"<< std::endl;
        break;
        case TokenKind::STRING:
            std::cout << "(Token: STRING, Content: '" << text << "')"<< std::endl;
        break;
        default:
            std::cout << "(Token: KEYWORD, Content: '" << text << "')"<< std::endl;
            break;
    }
}

void Lexer::Lexer::debug(const std::vector<Token>& tokens) {
    for (auto& token : tokens) {
        m_debug_print_tokens(token);
    }
}