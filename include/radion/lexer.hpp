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

