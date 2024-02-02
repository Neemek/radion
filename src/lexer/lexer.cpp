#include <iostream>
#include <cstring>
#include <vector>

#include "radion/lexer.hpp"

vector<Token> Lexer::lex(std::string src)
{

    vector<Token> tokens = vector<Token>();
    int pos = 0, start;

    string c, n;
    while (pos < src.length()) {
        start = pos;
        c = src.at(pos);

        switch (c.at(0))
        {
        // Arithmetics
        case '+':
            tokens.push_back(Token(TokenType::PLUS, c, start));
            break;
        case '-':
            tokens.push_back(Token(TokenType::MINUS, c, start));
            break;
        case '*':
            tokens.push_back(Token(TokenType::STAR, c, start));
            break;
        case '/':
            tokens.push_back(Token(TokenType::SLASH, c, start));
            break;
        
        // Truthyness
        case '=':
            if (pos+1 < src.length() && src.at(++pos) == '=')
                tokens.push_back(Token(TokenType::EQUAL, "==", start));
            else
                tokens.push_back(Token(TokenType::ASSIGN, c, start));
            break;
        case '!':
            if (pos+1 < src.length() && src.at(++pos) == '=')
                tokens.push_back(Token(TokenType::NOT_EQUAL, c, start));
            else 
                tokens.push_back(Token(TokenType::NOT, "!=", start));
            break;
        case '>':
            if (pos+1 < src.length() && src.at(++pos) == '=')
                tokens.push_back(Token(TokenType::GREATER_EQUAL, ">=", start));
            else 
                tokens.push_back(Token(TokenType::GREATER_THAN, c, start));
            break;
        case '<':
            if (pos+1 < src.length() && src.at(++pos) == '='){
                tokens.push_back(Token(TokenType::LESS_EQUAL, "<=", start));
            } else 
                tokens.push_back(Token(TokenType::LESS_THAN, c, start));
            break;
        
        case '%':
            tokens.push_back(Token(TokenType::PERCENTAGE, c, start));
            break;
        
        case '"':
            {
                string str;
                str += '"';
                while (pos+1 < src.length() && src.at(pos+1) != '"') {
                    str += src.at(++pos);
                }
                pos++;
                str += '"';
                tokens.push_back(Token(TokenType::STRING, str, start));
            }
            break;

        // Parenthasis
        case '(':
            tokens.push_back(Token(TokenType::OPEN_PAREN, c, start));
            break;
        case ')':
            tokens.push_back(Token(TokenType::CLOSE_PAREN, c, start));
            break;
        case '[':
            tokens.push_back(Token(TokenType::OPEN_BRACKET, c, start));
            break;
        case ']':
            tokens.push_back(Token(TokenType::CLOSE_BRACKET, c, start));
            break;
        case '{':
            tokens.push_back(Token(TokenType::OPEN_CURLY, c, start));
            break;
        case '}':
            tokens.push_back(Token(TokenType::CLOSE_CURLY, c, start));
            break;

        case ',':
            tokens.push_back(Token(TokenType::COMMA, c, start));
            break;

        // Longer tokens
        default:
            if (isdigit(c.at(0))) {
                string num;
                num += c;
                while (pos+1 < src.length() && isdigit(src.at(pos+1))) {
                    num += src.at(++pos);
                }
                tokens.push_back(Token(TokenType::NUMBER, num, start));
            } else if (isalpha(c.at(0))) {
                string v;
                v += c;
                while (pos+1 < src.length() && (isalpha(src.at(pos+1)) || (const char*)src.at(pos+1) == "_")) {
                    v += src.at(++pos);
                }

                const char* var = v.c_str();
                // conditionals
                if (!strcmp(var, "if")) tokens.push_back(Token(TokenType::IF, var, start));
                else if (!strcmp(var, "else")) tokens.push_back(Token(TokenType::ELSE, var, start));
                // Literal values
                else if (!strcmp(var, "true")) tokens.push_back(Token(TokenType::TRUE, var, start));
                else if (!strcmp(var, "false")) tokens.push_back(Token(TokenType::FALSE, var, start));
                else if (!strcmp(var, "nil")) tokens.push_back(Token(TokenType::NIL, var, start));
                // Loops
                else if (!strcmp(var, "for")) tokens.push_back(Token(TokenType::FOR, var, start));
                else if (!strcmp(var, "while")) tokens.push_back(Token(TokenType::WHILE, var, start));
                // Function
                else if (!strcmp(var, "func")) tokens.push_back(Token(TokenType::FUNC, var, start));
                else if (!strcmp(var, "return")) tokens.push_back(Token(TokenType::RETURN, var, start));
                // Variable/name
                else tokens.push_back(Token(TokenType::NAME, var, start));
            }
            break;
        }

        pos++;
    }
    tokens.push_back(Token(TokenType::END, "", pos+1));

    return tokens;
}

void print_tokens(vector<Token> tokens) {
    for (Token token : tokens) {
        std::cout << token.pos() << ": " << token.type() << " => " << token.lexeme() << std::endl;
    }
}
