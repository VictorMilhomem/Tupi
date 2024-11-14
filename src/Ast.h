#ifndef AST_H
#define AST_H
#include <iostream>
#include <memory>
#include <variant>


namespace Ast {


    struct Expression;

    struct Constant {
        int value;
    };

    struct Unary {
        std::string op;
        std::unique_ptr<Expression> expression;
        Unary(std::string op, std::unique_ptr<Expression> expression) : op(std::move(op)), expression(std::move(expression)) {}
    };

    struct Expression {
        std::variant<std::unique_ptr<Unary>, std::unique_ptr<Constant>> content;

        explicit Expression(std::unique_ptr<Unary> u) : content(std::move(u)) {}
        explicit Expression(std::unique_ptr<Constant> c) : content(std::move(c)) {}
    };

    struct Statement {
        virtual ~Statement() = default;
    };

    struct ReturnStmt final : public Statement {
        std::unique_ptr<Expression> expression;
        explicit ReturnStmt(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}
    };

    struct Function {
        std::string name;
        std::unique_ptr<Statement> body;
        Function(std::string name, std::unique_ptr<Statement> body) : name(std::move(name)), body(std::move(body)) {}
    };

    struct Program {
        std::unique_ptr<Function> function;
        explicit Program(std::unique_ptr<Function> func) : function(std::move(func)) {}
    };

    class PrettyPrinter {
    public:
        void printProgram(const Program& program) {
            std::cout << "{\n";
            printFunction(*program.function, 1);
            std::cout << "}\n";
        }

    private:

        void printFunction(const Function& function, int indentLevel) {
            printIndent(indentLevel);
            std::cout << "\"function\": {\n";
            printIndent(indentLevel + 1);
            std::cout << "\"name\": \"" << function.name << "\",\n";
            printIndent(indentLevel + 1);
            std::cout << "\"body\": ";
            printStatement(*function.body, indentLevel + 1);
            std::cout << "\n";
            printIndent(indentLevel);
            std::cout << "}";
        }

        void printStatement(const Statement& stmt, int indentLevel) {
            const auto* returnStmt = dynamic_cast<const ReturnStmt*>(&stmt);
            if (returnStmt) {
                std::cout << "{\n";
                printIndent(indentLevel + 1);
                std::cout << "\"type\": \"ReturnStmt\",\n";
                printIndent(indentLevel + 1);
                std::cout << "\"expression\": ";
                printExpression(*returnStmt->expression, indentLevel + 1);
                std::cout << "\n";
                printIndent(indentLevel);
                std::cout << "}";
            } else {
                std::cerr << "Unknown statement type!\n";
            }
        }

        void printExpression(const Expression& expr, int indentLevel) {
            if (auto unary = std::get_if<std::unique_ptr<Unary>>(&expr.content)) {
                printUnary(**unary, indentLevel);
            } else if (auto constant = std::get_if<std::unique_ptr<Constant>>(&expr.content)) {
                printConstant(**constant, indentLevel);
            } else {
                std::cerr << "Unknown expression type!\n";
            }
        }

        void printUnary(const Unary& unary, int indentLevel) {
            std::cout << "{\n";
            printIndent(indentLevel + 1);
            std::cout << "\"type\": \"Unary\",\n";
            printIndent(indentLevel + 1);
            std::cout << "\"op\": \"" << unary.op << "\",\n";
            printIndent(indentLevel + 1);
            std::cout << "\"expression\": ";
            printExpression(*unary.expression, indentLevel + 1);
            std::cout << "\n";
            printIndent(indentLevel);
            std::cout << "}";
        }

        void printConstant(const Constant& constant, int indentLevel) {
            std::cout << "{\n";
            printIndent(indentLevel + 1);
            std::cout << "\"type\": \"Constant\",\n";
            printIndent(indentLevel + 1);
            std::cout << "\"value\": " << constant.value << "\n";
            printIndent(indentLevel);
            std::cout << "}";
        }

        void printIndent(int indentLevel) {
            for (int i = 0; i < indentLevel; ++i) {
                std::cout << "  ";
            }
        }
    };

    void test();

}

#endif //AST_H