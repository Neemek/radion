#pragma once

#include "tokens.hpp"
#include <string>
using namespace std;

class Token
{
public:
	Token(TokenType type, string lexeme, unsigned int pos);

	// Get the type of this token
	TokenType type();

	// Get the lexeme
	string lexeme();

	// Get the position of the lexeme in the source
	[[nodiscard]] unsigned int pos() const;

private:
	TokenType _type;
	string _lexeme;
	unsigned int _pos;
};

std::string ttype_to_string(TokenType type);
