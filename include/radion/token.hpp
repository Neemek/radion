#pragma once

#include "tokens.hpp"
#include <string>
using namespace std;

class Token
{
public:
	Token(TokenType type, string lexeme, int pos);

	// Get the type of this token
	TokenType type();

	// Get the lexeme
	string lexeme();

	// Get the position of the lexeme in the source
	int pos();

private:
	TokenType _type;
	string _lexeme;
	int _pos;
};
