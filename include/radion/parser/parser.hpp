#pragma once
#include "radion/parser/node.hpp"
#include "radion/token.hpp"
#include "radion/parser/nodes/block.hpp"
#include <string>

class Parser {
public:
    Parser();
    Parser(vector<Token> tokens, string src);
    BlockNode* parse();
    bool hadError = false;

    void reset(vector<Token> tokens, string src);

private:
    int pos = -1;
    std::string src;
    Token* prev;
    Token* curr;
    vector<Token> tokens;

    uint32_t start = 0;
    uint32_t end = 0;

    void nextToken();
    void error(Token* token, const char* message);

    bool accept(TokenType type);
    void expect(TokenType type, const char* error);

    Node* factor();
    Node* range();
    Node* term();
    Node* product();
    Node* expression();
    Node* statement();
    Node* block();
};

std::string get_position_descriptor(std::string src, int pos);
