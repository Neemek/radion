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

    void reset(vector<Token> tokenss, string src);

private:
    unsigned int pos = 0;
    std::string src;
    Token* prev{};
    Token* curr{};
    vector<Token> tokens;

    unsigned int start = 0;
    unsigned int prev_end = 0;

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

class ParsingException : std::exception {
public:
    explicit ParsingException(const char* message);
    [[nodiscard]] const char * what() const noexcept override;
private:
    const char* message;
};

std::string get_position_descriptor(std::string src, unsigned int pos);
