#pragma once

#include "tokens.hpp"
#include <string>
using namespace std;

class Token
{
public:
	Token(TokenType type, char lexeme[], int pos);

	// Get the type of this token
	TokenType type();

	// Get the lexeme
	std::string lexeme();

	// Get the position of the lexeme in the source
	int pos();

private:
	TokenType _type;
	char* _lexeme;
	int _pos;
};
