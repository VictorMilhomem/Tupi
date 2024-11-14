#include "CodeGen.h"


void print_indent(int indent_level) {
    for (int i = 0; i < indent_level; ++i) {
        std::cout << "  ";  // Two spaces per indent level
    }
}


void print_key_value(const std::string& key, const std::string& value, int indent_level, bool is_last = false) {
    print_indent(indent_level);
    std::cout << "\"" << key << "\": \"" << value << "\"" << (is_last ? "\n" : ",\n");
}


void print_constant(const Assembly::Constant& constant, int indent_level) {
    print_indent(indent_level);
    std::cout << "{\n";
    print_key_value("type", "Constant", indent_level + 1);
    print_key_value("value", std::to_string(constant.value), indent_level + 1, true);
    print_indent(indent_level);
    std::cout << "}";
}


void print_var(const Assembly::Var& var, int indent_level) {
    print_indent(indent_level);
    std::cout << "{\n";
    print_key_value("type", "Var", indent_level + 1);
    print_key_value("name", var.name, indent_level + 1, true);
    print_indent(indent_level);
    std::cout << "}";
}


void print_val(const Assembly::Val& val, int indent_level) {
    if (auto* constant = std::get_if<std::unique_ptr<Assembly::Constant>>(&val.type)) {
        print_constant(**constant, indent_level);
    } else if (auto* var = std::get_if<std::unique_ptr<Assembly::Var>>(&val.type)) {
        print_var(**var, indent_level);
    }
}


void print_unary_op(const Assembly::UnaryOp& unary_op, int indent_level) {
    print_indent(indent_level);
    std::cout << "\"op\": ";
    if (std::get_if<std::unique_ptr<Assembly::Negate>>(&unary_op.type)) {
        std::cout << "\"Negate\"";
    } else if (std::get_if<std::unique_ptr<Assembly::Complement>>(&unary_op.type)) {
        std::cout << "\"Complement\"";
    }
    std::cout << ",\n";
}


void print_unary(const Assembly::Unary& unary, int indent_level) {
    print_indent(indent_level);
    std::cout << "{\n";
    print_indent(indent_level + 1);
    std::cout << "\"type\": \"Unary\",\n";
    print_unary_op(*unary.op, indent_level + 1);
    print_indent(indent_level + 1);
    std::cout << "\"src\": ";
    print_val(*unary.src, 0);  // src is Val
    std::cout << ",\n";
    print_indent(indent_level + 1);
    std::cout << "\"dest\": ";
    print_val(*unary.dest, 0);  // dest is Val
    std::cout << "\n";
    print_indent(indent_level);
    std::cout << "}";
}


void print_return(const Assembly::Return& ret, int indent_level) {
    print_indent(indent_level);
    std::cout << "{\n";
    print_key_value("type", "Return", indent_level + 1);
    print_indent(indent_level + 1);
    std::cout << "\"value\": ";
    print_val(*ret.type, 0);  // return value is a Val
    std::cout << "\n";
    print_indent(indent_level);
    std::cout << "}";
}


void print_instruction(const Assembly::Instruction& instruction, int indent_level) {
    if (auto* ret = std::get_if<std::unique_ptr<Assembly::Return>>(&instruction.type)) {
        print_return(**ret, indent_level);
    } else if (auto* unary = std::get_if<std::unique_ptr<Assembly::Unary>>(&instruction.type)) {
        print_unary(**unary, indent_level);
    }
}


void print_function(const Assembly::Function& function, int indent_level) {
    print_indent(indent_level);
    std::cout << "{\n";
    print_key_value("name", function.name, indent_level + 1);
    print_indent(indent_level + 1);
    std::cout << "\"body\": [\n";
    for (size_t i = 0; i < function.body.size(); ++i) {
        print_instruction(*function.body[i], indent_level + 2);
        if (i < function.body.size() - 1) {
            std::cout << ",\n";
        }
    }
    std::cout << "\n";
    print_indent(indent_level + 1);
    std::cout << "]\n";
    print_indent(indent_level);
    std::cout << "}";
}

void print_program(const Assembly::Program& program, int indent_level = 0) {
    std::cout << "{\n";
    print_indent(indent_level + 1);
    std::cout << "\"function_def\": ";
    print_function(*program.function_def->function, indent_level + 1);
    std::cout << "\n";
    print_indent(indent_level);
    std::cout << "}\n";
}