#pragma once
#include "radion/parser/node.hpp"
#include "radion/token.hpp"
#include "radion/parser/nodes/block.hpp"
#include <string>

class Parser {
public:
    Parser();
    Parser(vector<Token> tokens, string src);
    BlockNode parse();
    bool hadError = false;

    void reset(vector<Token> tokens, string src);

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
    void expect(TokenType type, const char* error);

    Node* factor();
    Node* term();
    Node* expression();
    Node* statement();
    Node* block();
};