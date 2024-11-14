#include <iostream>
#include "src/Lexer.h"
#include "src/Ast.h"
#include "src/CodeGen.h"
#include "src/Parser.h"

int main() {
    std::string input = "fn main() : int {  return -2; }";
    auto lexer = Lexer::Lexer(input);
    auto tokens = lexer.tokenize();
    std::cout << "============ Lexer Tokens ============" << std::endl;
    Lexer::Lexer::debug(tokens);
    auto parser = Parser::Parser(tokens);
    auto ast = parser.parseProgram();
    const auto printer = std::make_unique<Ast::PrettyPrinter>();
    std::cout << "============ Parsing Tree ============" << std::endl;
    printer->printProgram(*ast);


    std::cout << "============ Assembly Tree ============" << std::endl;
    // Print the generated assembly

    std::cout << "============ Code Generated ============" << std::endl;
    auto program = std::make_unique<Assembly::Program>();



    return 0;
}
