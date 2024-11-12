#include "Parser.h"

/*
 * SIMPLE GRAMMAR
<program> ::= <function>
<function> ::= "fn" <id> "(" ")" ":" Type "{" <statement> "}"
<statement> ::= "return" <exp> ";"
<exp> ::= <unary_op> <exp> | <int>
<unary_op> ::= "!" | "~" | "-"
 */

std::unique_ptr<Ast::Constant> Parser::Parser::parseConstant() {
    auto token = getCurrentToken();
    std::string value_str = token.text;
    int value = std::stoi(value_str);

    return std::move(std::make_unique<Ast::Constant>(value));
}


std::unique_ptr<Ast::Unary> Parser::Parser::parseUnary() {
    auto token_op = getPreviousToken();
    auto token = getCurrentToken();
    std::string op = token_op.text;
    // for now we are parsing only int expr
    match(token, Lexer::TokenKind::INTEGER);
    std::unique_ptr<Ast::Expression> expr = parseExpression();
    return std::make_unique<Ast::Unary>(op, std::move(expr));

}


std::unique_ptr<Ast::Expression> Parser::Parser::parseExpression() {
    auto token = getCurrentToken();

    if (token.kind == Lexer::TokenKind::INTEGER) {
        auto integer = parseConstant();
        match(token, Lexer::TokenKind::INTEGER);
        return std::make_unique<Ast::Expression>(std::move(integer));
    }
    if (token.kind == Lexer::TokenKind::BITWISE ||
        token.kind == Lexer::TokenKind::NOT ||
        token.kind == Lexer::TokenKind::MINUS) {
        auto unary = parseUnary();
        return std::make_unique<Ast::Expression>(std::move(unary));
    }

    if (token.kind == Lexer::TokenKind::LPAREN) {
        auto expr = parseExpression();
        match(token, Lexer::TokenKind::RPAREN);
        return std::move(expr);
    }
    throw std::runtime_error("Malformed expression");

}

std::unique_ptr<Ast::Statement> Parser::Parser::parseStatement() {
    auto token = getCurrentToken();
    std::unique_ptr<Ast::Expression> expr;
    match(token, Lexer::TokenKind::RETURN);
    expr = parseExpression();
    token = getCurrentToken();

    match(token, Lexer::TokenKind::SEMICOLON);
    return std::make_unique<Ast::ReturnStmt>(std::move(expr));
}


std::unique_ptr<Ast::Function> Parser::Parser::parseFunction() {
    auto token = getCurrentToken();
    std::string name = token.text;
    match(token, Lexer::TokenKind::IDENTIFIER);
    token = getCurrentToken();
    match(token, Lexer::TokenKind::LPAREN);
    token = getCurrentToken();
    // after match the arguments
    match(token, Lexer::TokenKind::RPAREN);
    token = getCurrentToken();
    match(token, Lexer::TokenKind::COLON);
    token = getCurrentToken();
    match(token, Lexer::TokenKind::TYPE);
    token = getCurrentToken();
    match(token, Lexer::TokenKind::LBRACE);
    auto body = parseStatement();
    token = getCurrentToken();

    match(token, Lexer::TokenKind::RBRACE);

    return std::make_unique<Ast::Function>(name, std::move(body));
}


