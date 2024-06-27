cmake_minimum_required(VERSION 3.27)
project(radion)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)
add_executable(
    radion 
        src/main.cpp
        src/arguments.cpp
        src/lexer/lexer.cpp
        src/token/token.cpp
        src/parser/parser.cpp
        src/interpreter/interpreter.cpp
        src/interpreter/callable.cpp
        src/interpreter/exceptions.cpp
        src/interpreter/symboltable.cpp
        src/interpreter/stlib/stlib.cpp
        src/interpreter/value.cpp
        src/interpreter/values/string.cpp
        src/interpreter/values/constant.cpp
        src/interpreter/values/list.cpp
        src/interpreter/values/number.cpp
)

set_property(TARGET radion PROPERTY DEBUG_POSTFIX d)
set_property(TARGET radion PROPERTY CXX_STANDARD 17)
