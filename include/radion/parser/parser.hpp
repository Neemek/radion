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
    Node* block(bool capturing=false);
    bool hadError = false;

    void reset(vector<Token> tokens, string src);

private:
    int pos = -1;
    std::string src;
    Token* prev{};
    Token* curr{};
    vector<Token> tokens;

    int start = 0;
    int prev_end = 0;

    void nextToken();
    void error(Token* token, const char* message);

    bool accept(TokenType type);
    bool accept_seq(std::vector<TokenType> ts);
    void expect(TokenType type, const char* error);

    Node* factor();
    Node* range();
    Node* product();
    Node* expression();
    Node* comparison();
    Node* statement();
};

std::string get_position_descriptor(std::string src, int pos);
