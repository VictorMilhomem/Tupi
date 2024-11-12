#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "Ast.h"
#include "Lexer.h"

namespace Parser {
    class Parser {
    private:
        std::vector<Lexer::Token> tokens;
        size_t pos;

        std::unique_ptr<Ast::Function> parseFunction();
        std::unique_ptr<Ast::Statement> parseStatement();
        std::unique_ptr<Ast::Expression> parseExpression();
        std::unique_ptr<Ast::Unary> parseUnary();
        std::unique_ptr<Ast::Constant> parseConstant();

        Lexer::Token match(Lexer::Token& token, Lexer::TokenKind expected) {
            printf("Token: %s\n", token.text.c_str());

            if (token.kind != expected) {
                throw std::runtime_error("Syntax Error: Unexpected token " +
                    std::to_string(static_cast<int>(token.kind)) +
                    " (Expected: " + std::to_string(static_cast<int>(expected)) + ") " +
                    "at position: " + std::to_string(pos));
            }
            // Fetch and return the next token
            if (token.kind != Lexer::TokenKind::TEOF) {
                token = getNextToken();
            }
            return token;
        }


        Lexer::Token getCurrentToken() {
            return tokens[pos];
        }

        Lexer::Token getNextToken() {
            return tokens[pos++];
        }

        Lexer::Token getPreviousToken() {
            return tokens[pos - 1];
        }

    public:
        explicit Parser(const std::vector<Lexer::Token>& tokens) : tokens(tokens), pos(0) {}

        std::unique_ptr<Ast::Program> parseProgram() {
            auto token = getCurrentToken();
            match(token, Lexer::TokenKind::FN);
            auto fn = parseFunction();
            token = getCurrentToken();
            match(token, Lexer::TokenKind::TEOF);
            return std::make_unique<Ast::Program>(std::move(fn));
        }
    };
}

#endif //PARSER_H
