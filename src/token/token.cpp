#include "radion/token.hpp"
#include "radion/tokens.hpp"

#include <string>
#include <utility>

using namespace std;

Token::Token(TokenType type, string lexeme, int pos)
{
	_type = type;
	_lexeme = std::move(lexeme);
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

std::string ttype_to_string(TokenType type) {
	switch (type)
	{
	case TokenType::ASSIGN:
		return "ASSIGN";
	case TokenType::NAME:
		return "NAME";
	case TokenType::STRING:
		return "STRING";
	case TokenType::NUMBER:
		return "NUMBER";
	case TokenType::NIL:
		return "NIL";
	case TokenType::TRUE:
		return "TRUE";
	case TokenType::FALSE:
		return "FALSE";
	case TokenType::PLUS:
		return "PLUS";
	case TokenType::DOUBLE_PLUS:
		return "DOUBLE_PLUS";
	case TokenType::MINUS:
		return "MINUS";
	case TokenType::DOUBLE_MINUS:
		return "DOUBLE_MINUS";
	case TokenType::STAR:
		return "STAR";
    case TokenType::DOUBLE_STAR:
        return "DOUBLE_STAR";
	case TokenType::SLASH:
		return "SLASH";
	case TokenType::DOUBLE_SLASH:
		return "DOUBLE_SLASH";
	case TokenType::PERCENTAGE:
		return "PERCENTAGE";
	case TokenType::EQUAL:
		return "EQUAL";
	case TokenType::NOT_EQUAL:
		return "NOT_EQUAL";
	case TokenType::LESS_THAN:
		return "LESS_THAN";
	case TokenType::LESS_EQUAL:
		return "LESS_EQUAL";
	case TokenType::GREATER_THAN:
		return "GREATER_THAN";
	case TokenType::GREATER_EQUAL:
		return "GREATER_EQUAL";
	case TokenType::NOT:
		return "NOT";
	case TokenType::OPEN_PAREN:
		return "OPEN_PAREN";
	case TokenType::CLOSE_PAREN:
		return "CLOSE_PAREN";
	case TokenType::OPEN_BRACKET:
		return "OPEN_BRACKET";
	case TokenType::CLOSE_BRACKET:
		return "CLOSE_BRACKET";
	case TokenType::OPEN_CURLY:
		return "OPEN_CURLY";
	case TokenType::CLOSE_CURLY:
		return "CLOSE_CURLY";
	case TokenType::IF:
		return "IF";
	case TokenType::ELSE:
		return "ELSE";
	case TokenType::FOR:
		return "FOR";
	case TokenType::WHILE:
		return "WHILE";
	case TokenType::DO:
		return "DO";
	case TokenType::FUNC:
		return "FUNC";
	case TokenType::RETURN:
		return "RETURN";
	case TokenType::DOT:
		return "DOT";
	case TokenType::DOUBLE_DOT:
		return "DOUBLE_DOT";
	case TokenType::COMMA:
		return "COMMA";
	case TokenType::END:
		return "END";
    case TokenType::IN:
        return "IN";
	
	default:
		throw "Missing conversion from token type to string";
	}
}
