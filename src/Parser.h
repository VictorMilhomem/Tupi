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

        void match(Lexer::TokenKind token_kind, Lexer::TokenKind expected) {
            if (token_kind != expected) throw std::runtime_error("Syntax Error: Unexpected token");
            pos++;
        };

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
            return std::make_unique<Ast::Program>(parseFunction());
        }
    };
}

#endif //PARSER_H
