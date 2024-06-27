#include <iostream>
#include <cstring>
#include <vector>

#include "radion/lexer.hpp"
#include "radion/parser/parser.hpp"

vector<Token> Lexer::lex(std::string src)
{

    vector<Token> tokens = vector<Token>();
    unsigned int pos = 0, start;

    string c, n;
    while (pos < src.length()) {
        start = pos;
        c = src.at(pos);

        switch (c.at(0))
        {
        // Arithmetics
        case '+':
            if (pos+1 < src.length() && src.at(pos+1) == '+') {
                tokens.emplace_back(TokenType::DOUBLE_PLUS, "++", start);
                pos++;
            }
            else tokens.emplace_back(TokenType::PLUS, c, start);
            break;
        case '-':
            if (pos+1 < src.length() && src.at(pos+1) == '-') {
                tokens.emplace_back(TokenType::DOUBLE_MINUS, "--", start);
                pos++;
            }
            else tokens.emplace_back(TokenType::MINUS, c, start);
            break;
        case '*':
            if (pos+1 < src.length() && src.at(pos+1) == '*') {
                tokens.emplace_back(TokenType::DOUBLE_STAR, "**", start);
                pos++;
            }
            else tokens.emplace_back(TokenType::STAR, c, start);
            break;
        case '/':
            if (pos+1 < src.length() && src.at(pos+1) == '*') {
                do {
                    pos++;
                } while (pos+1 < src.length() && !(src.at(pos) == '*' && src.at(pos+1) == '/'));
                pos++;
            } else if (pos+1 < src.length() && src.at(pos+1) == '/') {
                tokens.emplace_back(TokenType::DOUBLE_SLASH, "//", start);
                pos++;
            } else  {
                tokens.emplace_back(TokenType::SLASH, c, start);
            }
            break;
        
        // Truthyness
        case '=':
            if (pos+1 < src.length() && src.at(pos+1) == '=') {
                tokens.emplace_back(TokenType::EQUAL, "==", start);
                pos++;
            }
            else
                tokens.emplace_back(TokenType::ASSIGN, c, start);
            break;
        case '!':
            if (pos+1 < src.length() && src.at(pos+1) == '=') {
                tokens.emplace_back(TokenType::NOT_EQUAL, c, start);
                pos++;
            }
            else 
                tokens.emplace_back(TokenType::NOT, "!=", start);
            break;
        case '>':
            if (pos+1 < src.length() && src.at(pos+1) == '=') {
                tokens.emplace_back(TokenType::GREATER_EQUAL, ">=", start);
                pos++;
            }
            else 
                tokens.emplace_back(TokenType::GREATER_THAN, c, start);
            break;
        case '<':
            if (pos+1 < src.length() && src.at(pos+1) == '=') {
                tokens.emplace_back(TokenType::LESS_EQUAL, "<=", start);
                pos++;
            } else 
                tokens.emplace_back(TokenType::LESS_THAN, c, start);
            break;
        
        case '%':
            tokens.emplace_back(TokenType::PERCENTAGE, c, start);
            break;

        case '#':
            while (pos + 1 < src.size() && src.at(pos+1) != '\n') pos++;
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
                tokens.emplace_back(TokenType::STRING, str, start);
            }
            break;

        // Parenthasis
        case '(':
            tokens.emplace_back(TokenType::OPEN_PAREN, c, start);
            break;
        case ')':
            tokens.emplace_back(TokenType::CLOSE_PAREN, c, start);
            break;
        case '[':
            tokens.emplace_back(TokenType::OPEN_BRACKET, c, start);
            break;
        case ']':
            tokens.emplace_back(TokenType::CLOSE_BRACKET, c, start);
            break;
        case '{':
            tokens.emplace_back(TokenType::OPEN_CURLY, c, start);
            break;
        case '}':
            tokens.emplace_back(TokenType::CLOSE_CURLY, c, start);
            break;

        case '.':
            if (pos+1 < src.length() && src.at(pos+1) == '.') {
                tokens.emplace_back(TokenType::DOUBLE_DOT, "..", start);
                pos++;
            } else 
                tokens.emplace_back(TokenType::DOT, c, start);
            break;
        case ',':
            tokens.emplace_back(TokenType::COMMA, c, start);
            break;

        // Longer tokens
        default:
            if (isdigit(c.at(0))) {
                string num;
                num += c;
                while (pos+1 < src.length() && isdigit(src.at(pos+1))) {
                    num += src.at(++pos);
                }
                tokens.emplace_back(TokenType::NUMBER, num, start);
            } else if (isalpha(c.at(0))) {
                string v;
                v += c;
                while (pos+1 < src.length() && name_acceptable(src.at(pos+1))) {
                    v += src.at(++pos);
                }

                const char* var = v.c_str();
                // conditionals
                if (!strcmp(var, "if")) tokens.emplace_back(TokenType::IF, var, start);
                else if (!strcmp(var, "else")) tokens.emplace_back(TokenType::ELSE, var, start);
                // Literal values
                else if (!strcmp(var, "true")) tokens.emplace_back(TokenType::TRUE, var, start);
                else if (!strcmp(var, "false")) tokens.emplace_back(TokenType::FALSE, var, start);
                else if (!strcmp(var, "nil")) tokens.emplace_back(TokenType::NIL, var, start);
                // Loops
                else if (!strcmp(var, "for")) tokens.emplace_back(TokenType::FOR, var, start);
                else if (!strcmp(var, "in")) tokens.emplace_back(TokenType::IN, var, start);
                else if (!strcmp(var, "while")) tokens.emplace_back(TokenType::WHILE, var, start);
                else if (!strcmp(var, "do")) tokens.emplace_back(TokenType::DO, var, start);
                // Function
                else if (!strcmp(var, "func")) tokens.emplace_back(TokenType::FUNC, var, start);
                else if (!strcmp(var, "return")) tokens.emplace_back(TokenType::RETURN, var, start);
                // Variable/name
                else tokens.emplace_back(TokenType::NAME, var, start);
            }
            break;
        }

        pos++;
    }
    tokens.emplace_back(TokenType::END, "", pos+1);

    return tokens;
}

bool name_acceptable(char c) {
    return isalpha(c) || isdigit(c) || c == '_';
}

void print_tokens(const std::string& src, const vector<Token>& tokens) {
    for (Token token : tokens) {
        std::cout << get_position_descriptor(src, token.pos()) << "\t " << ttype_to_string(token.type()) << " => " << token.lexeme() << std::endl;
    }
}
