#include "radion/token.hpp"
#include "radion/tokens.hpp"

#include <string>

using namespace std;

Token::Token(TokenType type, string lexeme, int pos)
{
	_type = type;
	_lexeme = lexeme;
	_pos = pos;
}

TokenType Token::type()
{
	return _type;
}

string Token::lexeme()
{
	return _lexeme;
}

int Token::pos()
{
	return _pos;
}
