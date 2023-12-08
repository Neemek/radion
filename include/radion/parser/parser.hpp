#pragma once
#include "radion/parser/node.hpp"
#include "radion/token.hpp"
#include <string>

class Parser {
public:
    Parser(vector<Token> tokens, string src);
    Node parse();

private:
    int pos = -1;
    std::string src;
    Token* prev;
    Token* curr;
    vector<Token> tokens;

    void nextToken();
    Token* peek();
    void error(Token token, const char* message);

    bool accept(TokenType type);
    void expect(TokenType type);

    Node factor();
    Node term();
    Node expression();
    Node statement();
    Node block();
};