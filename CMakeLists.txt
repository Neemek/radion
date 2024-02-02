cmake_minimum_required(VERSION 3.27)
project(radion)

include_directories(include)
add_executable(
    radion 
    src/main.cpp 
    src/lexer/lexer.cpp src/token/token.cpp 
    src/parser/parser.cpp 
    src/interpreter/interpreter.cpp src/interpreter/callable.cpp
    src/interpreter/exceptions.cpp
)

set_property(TARGET radion PROPERTY DEBUG_POSTFIX d)
set_property(TARGET radion PROPERTY CXX_STANDARD 17)
