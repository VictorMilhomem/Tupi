#include <iostream>
#include "src/Lexer.h"
#include "src/Ast.h"
#include "src/CodeGen.h"
#include "src/Parser.h"

int main() {
    std::string input = "fn main() : int {  return -2; }";
    auto lexer = Lexer::Lexer(input);
    auto tokens = lexer.tokenize();
    Lexer::Lexer::debug(tokens);
    auto parser = Parser::Parser(tokens);
    auto ast = parser.parseProgram();
    const auto printer = std::make_unique<Ast::PrettyPrinter>();
    printer->printProgram(*ast);
    auto codegen = std::make_unique<CodeGen>(std::move(ast));  // Move ownership

    // Generate assembly code
    auto assembly = codegen->convert_program();

    // Print the generated assembly
    print_program(*assembly);

}
