#include "radion/parser/parser.hpp"
#include "radion/parser/node.hpp"
#include "radion/token.hpp"

#include "radion/parser/nodes/arithmetic.hpp"
#include "radion/parser/nodes/literal.hpp"
#include "radion/parser/nodes/reference.hpp"
#include "radion/parser/nodes/functions.hpp"
#include "radion/parser/nodes/conditionals.hpp"
#include "radion/parser/nodes/block.hpp"
#include "radion/parser/nodes/loops.hpp"


#include <iostream>

Parser::Parser(vector<Token> tokens, string src) {
    this->tokens = tokens;
    this->src = src;

    this->nextToken();
}

Node Parser::parse() {
    BlockNode n;

    while (this->curr->type() != TokenType::END)
        n.statements.push_back(this->block());
    
    return n;
};

void Parser::nextToken() {
    this->prev = curr;
    this->curr = &this->tokens[++this->pos];
}

void Parser::error(Token token, const char* message) {
    // do error handling
    std::cout << token.pos() << ":" << message << std::endl;
};

bool Parser::accept(TokenType type) {
    if (this->curr->type() == type) {
        this->nextToken();
        return true;
    }
    return false;
};

void Parser::expect(TokenType type) {
    if (!this->accept(type)) {
        this->error(*this->curr, "expect: unexpected token");
    }
};
Node Parser::factor() {
    if (this->accept(TokenType::STRING)) {
        StringLiteralNode n;

        n.string = this->prev->lexeme();

        return n;
    } else if (this->accept(TokenType::NUMBER)) {
        IntLiteralNode n;

        n.number = stoi(this->prev->lexeme());

        return n;
    } else if (this->accept(TokenType::TRUE) || this->accept(TokenType::FALSE)) {
        BooleanLiteralNode n;

        n.boolean = this->prev->type() == TokenType::TRUE;

        return n;
    } else if (this->accept(TokenType::NIL)) {
        return NilLiteralNode();
    } else if (this->accept(TokenType::NAME)) {
        std::string name = this->prev->lexeme();
        // Differentiate between reference and function call
        if (this->accept(TokenType::OPEN_PAREN)) {
            CallNode n;
            n.name = name;

            // Parse arguments 
            Node arg;
            for (int i = 0; i < MAX_PARAMS && this->curr->type() != TokenType::CLOSE_PAREN && (i > 0 || this->accept(TokenType::COMMA)); i++) {
                n.params[i] = this->factor();
            }

            if (this->accept(TokenType::CLOSE_PAREN)) return n;
            else this->error(*this->curr, "Max parameters reached");
        } else {
            ReferenceNode n;
            n.name = name;

            return n;
        }
    }
};

Node Parser::term() {
    Node f = this->factor();

    if (this->accept(TokenType::MULTIPLY) || this->accept(TokenType::DIVIDE)) {
        ArithmeticNode n;

        // Get operation
        n.op = this->prev->type() == TokenType::MULTIPLY 
            ? ArithmeticOperation::MULTIPLY : ArithmeticOperation::DIVIDE;
        n.left = f;

        n.right = this->term();

        return n;
    } else {
        return f;
    }
};

Node Parser::expression() {
    Node t = this->term();

    if (this->accept(TokenType::PLUS) || this->accept(TokenType::MINUS)) {
        ArithmeticNode n;

        // Get operation
        n.op = this->prev->type() == TokenType::PLUS 
            ? ArithmeticOperation::ADD : ArithmeticOperation::SUBTRACT;
        n.left = t;

        n.right = this->expression();

        return n;
    } else {
        return t;
    }
};

Node Parser::statement() {
    if (this->accept(TokenType::IF)) {
        IfNode n;

        n.condition = this->expression();
        n.logic = this->block();
        if (this->accept(TokenType::ELSE)) n.otherwise = this->block();

        return n;
    } else if (this->accept(TokenType::WHILE)) {
        LoopNode n;

        this->expect(TokenType::OPEN_PAREN);
        n.condition = this->expression();
        this->expect(TokenType::CLOSE_PAREN);

        n.logic = this->block();

        return n;
    } else if (this->accept(TokenType::FOR)) {
        LoopNode n;
    } else this->expression();
};

Node Parser::block() {
    if (this->accept(TokenType::OPEN_CURLY)) {
        BlockNode n;

        while (this->curr->type() != TokenType::CLOSE_CURLY)
            n.statements.push_back(this->statement());
        
        return n;
    } else this->statement();
};
