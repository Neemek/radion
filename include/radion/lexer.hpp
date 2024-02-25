#pragma once

#include "token.hpp"
#include "tokens.hpp"
#include <string>
#include <vector>
using namespace std;

class Lexer {
public:
	static vector<Token> lex(std::string src);
};

// if the character is acceptable, assuming the first character is alpha
bool name_acceptable(char c);

void print_tokens(const std::string& src, const vector<Token>& tokens);
