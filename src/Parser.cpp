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
    return std::make_unique<Ast::Constant>(value);
}


std::unique_ptr<Ast::Unary> Parser::Parser::parseUnary() {
    auto token_op = getPreviousToken();
    auto token = getCurrentToken();
    std::string op = token_op.text;
    // for now we are parsing only int expr
    match(token.kind, Lexer::TokenKind::INTEGER);
    std::unique_ptr<Ast::Expression> expr = parseExpression();
    return std::make_unique<Ast::Unary>(op, expr);

}


std::unique_ptr<Ast::Expression> Parser::Parser::parseExpression() {
    auto token = getCurrentToken();
    if (token.kind == Lexer::TokenKind::BITWISE ||
        token.kind == Lexer::TokenKind::NOT ||
        token.kind == Lexer::TokenKind::MINUS) {
        pos++;
        auto unary = parseUnary();
        return std::make_unique<Ast::Expression>(unary);
    }
    match(token.kind, Lexer::TokenKind::INTEGER);
    auto integer = parseConstant();
    return std::make_unique<Ast::Expression>(integer);
}

std::unique_ptr<Ast::Statement> Parser::Parser::parseStatement() {
    auto token = getCurrentToken();
    match(token.kind, Lexer::TokenKind::RETURN);
    auto expr = parseExpression();
    match(token.kind, Lexer::TokenKind::SEMICOLON);
    return std::make_unique<Ast::ReturnStmt>(std::move(expr));
}


std::unique_ptr<Ast::Function> Parser::Parser::parseFunction() {
    auto token = getNextToken();
    match(token.kind, Lexer::TokenKind::FN);
    match(token.kind, Lexer::TokenKind::IDENTIFIER);
    std::string name = token.text;
    match(token.kind, Lexer::TokenKind::LPAREN);
    // after match the arguments
    match(token.kind, Lexer::TokenKind::RPAREN);
    match(token.kind, Lexer::TokenKind::COLON);
    match(token.kind, Lexer::TokenKind::TYPE);
    match(token.kind, Lexer::TokenKind::LBRACE);
    // parse the function body
    auto body = parseStatement();
    match(token.kind, Lexer::TokenKind::RBRACE);
    return std::make_unique<Ast::Function>(name, body);
}


