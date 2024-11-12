#include <iostream>
#include "src/Lexer.h"
#include "src/Ast.h"
#include "src/Parser.h"

int main() {
    std::string input = "fn main() : int {  return 2; }";
    auto lexer = Lexer::Lexer(input);
    auto tokens = lexer.tokenize();
    Lexer::Lexer::debug(tokens);
    auto parser = Parser::Parser(tokens);
    auto ast = parser.parseProgram();


}
