project(radion)


include_directories(include)
add_executable(radion src/main.cpp src/lexer/lexer.cpp src/token/token.cpp src/parser/node.cpp src/parser/parser.cpp)
