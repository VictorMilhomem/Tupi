#include "Ast.h"

void Ast::test() {
    auto expr = std::make_unique<Expression>(
    std::make_unique<Unary>("-", std::make_unique<Expression>(std::make_unique<Constant>(5)))
    );

    // Create the return statement: return -5;
    auto stmt = std::make_unique<ReturnStmt>(std::move(expr));

    // Create the function: int main() { return -5; }
    auto func = std::make_unique<Function>("main", std::move(stmt));

    // Create the program
    const Program program(std::move(func));

    const auto printer = std::make_unique<PrettyPrinter>();
    printer->printProgram(program);
}
