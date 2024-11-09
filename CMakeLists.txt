cmake_minimum_required(VERSION 3.29)
project(Compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(Compiler main.cpp
        src/Lexer.cpp
        src/Lexer.h
        src/Parser.cpp
        src/Parser.h
        src/Ast.cpp
        src/Ast.h
        src/CodeGen.cpp
        src/CodeGen.h
        src/Assembly.cpp
        src/Assembly.h)
