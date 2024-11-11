#include <iostream>
#include "src/Lexer.h"
#include "src/Ast.h"

int main() {
    std::string input = "fn main(void) : int {  echo(\"Hello World!\") return 0; }";
    auto lexer = Lexer::Lexer(input);
    auto tokens = lexer.tokenize();

    Lexer::Lexer::debug(tokens);

    Ast::test();
}
