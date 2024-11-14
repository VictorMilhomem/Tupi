#ifndef CODEGEN_H
#define CODEGEN_H
#include <type_traits>
#include "Assembly.h"
#include "Ast.h"

#include <iostream>
#include <string>

void print_indent(int level);
void print_key_value(const std::string& key, const std::string& value, int level, bool indent);
void print_constant(const Assembly::Constant& constant, int level);
void print_var(const Assembly::Var& var, int level);
void print_val(const Assembly::Val& val, int level);
void print_unary_op(const Assembly::UnaryOp& op, int level);
void print_unary(const Assembly::Unary& unary, int level);
void print_return(const Assembly::Return& ret, int level);
void print_instruction(const Assembly::Instruction& instruction, int level);
void print_function(const Assembly::Function& function, int level);
void print_program(const Assembly::Program& program, int level= 0);


class CodeGen {
private:
    std::unique_ptr<Ast::Program> program;
public:
    CodeGen(std::unique_ptr<Ast::Program> program) : program(std::move(program)) {};

    std::unique_ptr<Assembly::Val> convert_constant(const Ast::Constant& constant) {
        return std::make_unique<Assembly::Val>(std::make_unique<Assembly::Constant>(constant.value));
    }


    std::unique_ptr<Assembly::Instruction> convert_unary(const Ast::Unary& unary_expr) {
        std::unique_ptr<Assembly::UnaryOp> op;
        if (unary_expr.op == "-") {
            op = std::make_unique<Assembly::UnaryOp>(std::make_unique<Assembly::Negate>());
        } else if (unary_expr.op == "~") {
            op = std::make_unique<Assembly::UnaryOp>(std::make_unique<Assembly::Complement>());
        }

        auto src = convert_expression(*unary_expr.expression);
        auto dest = std::make_unique<Assembly::Val>(std::make_unique<Assembly::Constant>(0));  // Placeholder destination

        auto unary = std::make_unique<Assembly::Unary>(std::move(op), std::move(src), std::move(dest));
        return std::make_unique<Assembly::Instruction>(std::move(unary));
    }


    std::unique_ptr<Assembly::Val> convert_expression(const Ast::Expression& expr) {
        if (auto constant = std::get_if<std::unique_ptr<Ast::Constant>>(&expr.content)) {
            return convert_constant(**constant);
        } else if (auto unary = std::get_if<std::unique_ptr<Ast::Unary>>(&expr.content)) {
            return std::make_unique<Assembly::Val>(std::make_unique<Assembly::Constant>(0)); // Placeholder for unary conversion
        }
        return nullptr;  // Error case
    }

    std::unique_ptr<Assembly::Instruction> convert_return(const Ast::ReturnStmt& return_) {
        auto val = convert_expression(*return_.expression);
        auto ret = std::make_unique<Assembly::Return>(std::move(val));
        return std::make_unique<Assembly::Instruction>(std::move(ret));
    };


    std::vector<std::unique_ptr<Assembly::Instruction>> convert_instruction(const Ast::Statement& stmt) {
        auto instructions = std::vector<std::unique_ptr<Assembly::Instruction>>();
        if (const auto* returnStmt = dynamic_cast<const Ast::ReturnStmt*>(&stmt)) {
            instructions.push_back(convert_return(*returnStmt));
        }
        return instructions;
    }


    std::unique_ptr<Assembly::FunctionDefinition> convert_function(const Ast::Function& function) {
        auto name = function.name;
        auto inst = convert_instruction(*function.body);
        auto func = std::make_unique<Assembly::Function>(std::move(name), std::move(inst));
        // Assuming FunctionDefinition takes a name and instructions
        return std::make_unique<Assembly::FunctionDefinition>(std::move(func));  // Return a valid unique_ptr
    }



    std::unique_ptr<Assembly::Program> convert_program() {
        // Ensure the function is not null before attempting to dereference
        if (program->function) {
            auto prog = convert_function(*program->function);
            return std::make_unique<Assembly::Program>(std::move(prog));
        } else {
            // Handle error or return an empty program
            throw std::runtime_error("Program doesn't have a function");
        }
    };

};



#endif //CODEGEN_H
