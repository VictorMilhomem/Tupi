#ifndef ASSEMBLY_H
#define ASSEMBLY_H
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Ast.h"


namespace Assembly {

    struct Complement {};
    struct Negate {};

    struct UnaryOp {
        std::variant<std::unique_ptr<Complement>,std::unique_ptr<Negate>> type;
        explicit UnaryOp(std::unique_ptr<Complement> type) : type(std::move(type)) {}
        explicit UnaryOp(std::unique_ptr<Negate> type) : type(std::move(type)) {}
    };

    struct Constant {
        int value;
        explicit Constant(int value) : value(value) {}
    };

    struct Var {
        std::string name;
        explicit Var(std::string name) : name(std::move(name)) {}
    };

    struct Val {
        std::variant<std::unique_ptr<Constant>,std::unique_ptr<Var>> type;
        explicit Val(std::unique_ptr<Constant> type) : type(std::move(type)) {}
        explicit Val(std::unique_ptr<Var> type) : type(std::move(type)) {}
    };

    struct Return {
        std::unique_ptr<Val> type;
        explicit Return(std::unique_ptr<Val> type) : type(std::move(type)) {}
    };

    struct Unary {
        std::unique_ptr<UnaryOp> op;
        std::unique_ptr<Val> src;
        std::unique_ptr<Val> dest;
        explicit Unary(std::unique_ptr<UnaryOp> op, std::unique_ptr<Val> src, std::unique_ptr<Val> dest) : op(std::move(op)), src(std::move(src)), dest(std::move(dest)) {}
    };


    struct Instruction {
        std::variant<std::unique_ptr<Return>, std::unique_ptr<Unary>> type;

        explicit Instruction(std::unique_ptr<Return> type) : type(std::move(type)) {}
        explicit Instruction(std::unique_ptr<Unary> type) : type(std::move(type)) {}
    };


    struct Function {
        std::string name;
        std::vector<std::unique_ptr<Instruction>> body;
        explicit Function(std::string name, std::vector<std::unique_ptr<Instruction>> body) : name(std::move(name)), body(std::move(body)) {}
    };

    struct FunctionDefinition {
        std::unique_ptr<Function> function;
    };

    struct Program {
        std::unique_ptr<FunctionDefinition>function_def;
        explicit Program(std::unique_ptr<FunctionDefinition> function_def) : function_def(std::move(function_def)) {}
        Program() : function_def(nullptr) {}
    };


}





#endif //ASSEMBLY_H
