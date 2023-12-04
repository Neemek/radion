#include "radion/lexer.hpp"

#include <vector>

vector<Token> Lexer::lex(char* src)
{
    string s(src);

    vector<Token> tokens = vector<Token>();
    int pos = 0;

    char c, n;
    while (pos < s.length()) {
        c = s.at(pos);

        switch (c)
        {
        // Arithmetics
        case '+':
            tokens.push_back(Token(TokenType::PLUS, &c, pos));
            break;
        case '-':
            tokens.push_back(Token(TokenType::MINUS, &c, pos));
            break;
        case '*':
            tokens.push_back(Token(TokenType::MULTIPLY, &c, pos));
            break;
        case '/':
            tokens.push_back(Token(TokenType::DIVIDE, &c, pos));
            break;
        
        // Truthyness
        case '=':
            tokens.push_back(Token(TokenType::EQUAL, &c, pos));
            break;
        case '!':
            n = s.at(pos+1);
            if (n == '=')
                tokens.push_back(Token(TokenType::NOT_EQUAL, &c, pos));
            else 
                tokens.push_back(Token(TokenType::NOT, "!=", pos));
            break;
        case '>':
            n = s.at(pos+1);
            if (n == '=')
                tokens.push_back(Token(TokenType::GREATER_EQUAL, ">=", pos));
            else 
                tokens.push_back(Token(TokenType::GREATER_THAN, &c, pos));
            break;
        case '<':
            n = s.at(pos+1);
            if (n == '='){
                tokens.push_back(Token(TokenType::LESS_EQUAL, "<=", pos));
            } else 
                tokens.push_back(Token(TokenType::LESS_THAN, &c, pos));
            break;
        
        case '%':
            tokens.push_back(Token(TokenType::MODULO, &c, pos));
            break;
        


        default:
            if (isdigit(c)) {
                int num = c - '0';
                while (isdigit(s[pos+1])) {
                    num += s[++pos] - '0';
                }
            }
            break;
        }
    }

    return tokens;
}