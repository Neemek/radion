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

Parser::Parser() {}

Parser::Parser(vector<Token> tokens, string src) {
    this->reset(tokens, src);
}

void Parser::reset(vector<Token> tokens, string src) {
    this->tokens = tokens;
    this->src = src;

    this->nextToken();
}

BlockNode Parser::parse() {
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
    std::cout << token.pos() << ": " << token.lexeme() << " " << message << std::endl;
    this->hadError = true;
};

bool Parser::accept(TokenType type) {
    if (this->curr->type() == type) {
        this->nextToken();
        return true;
    }
    return false;
};

void Parser::expect(TokenType type, const char* error) {
    if (!this->accept(type)) {
        this->error(*this->curr, error);
        this->nextToken();
    }
};

Node* Parser::factor() {
    if (this->accept(TokenType::STRING)) {
        StringLiteralNode* n = new StringLiteralNode;

        n->string = this->prev->lexeme().substr(1, this->prev->lexeme().length()-2);

        return n;
    } else if (this->accept(TokenType::NUMBER)) {
        IntLiteralNode* n = new IntLiteralNode;

        n->number = stoi(this->prev->lexeme());

        return n;
    } else if (this->accept(TokenType::TRUE) || this->accept(TokenType::FALSE)) {
        BooleanLiteralNode* n = new BooleanLiteralNode;

        n->boolean = this->prev->type() == TokenType::TRUE;

        return n;
    } else if (this->accept(TokenType::NIL)) {
        return new NilLiteralNode;
    } else if (this->accept(TokenType::NAME)) {
        std::string name = this->prev->lexeme();
        // Differentiate between reference and function call
        if (this->accept(TokenType::OPEN_PAREN)) {
            CallNode* n = new CallNode;
            n->name = name;

            // Parse arguments 
            int i = 0;
            do {
                n->params[i] = this->factor();
            } while (++i < MAX_PARAMS && this->accept(TokenType::COMMA));

            this->expect(TokenType::CLOSE_PAREN, (std::string("Maximum amount of parameters is ") + to_string(MAX_PARAMS)).c_str());
            return n;
            
        } else {
            ReferenceNode* n = new ReferenceNode;
            n->name = name;

            return n;
        }
    } else {
        this->error(*this->curr, "factor: invalid token");
        this->nextToken();
        return new NilLiteralNode();
    }
};

Node* Parser::term() {
    Node* f = this->factor();

    if (this->accept(TokenType::STAR) || this->accept(TokenType::SLASH)) {
        ArithmeticNode* n = new ArithmeticNode;

        // Get operation
        n->op = this->prev->type() == TokenType::STAR 
            ? ArithmeticOperation::MULTIPLY : ArithmeticOperation::DIVIDE;
        n->left = f;

        n->right = this->term();

        return n;
    } else {
        return f;
    }
};

Node* Parser::expression() {
    Node* t = this->term();

    if (this->accept(TokenType::PLUS) || this->accept(TokenType::MINUS)) {
        ArithmeticNode* n = new ArithmeticNode;

        // Get operation
        n->op = this->prev->type() == TokenType::PLUS 
            ? ArithmeticOperation::ADD : ArithmeticOperation::SUBTRACT;
        n->left = t;

        n->right = this->expression();

        return n;
    } else {
        return t;
    }
};

Node* Parser::statement() {
    if (this->accept(TokenType::IF)) {
        IfNode* n = new IfNode;

        n->condition = this->expression();
        n->logic = this->block();
        if (this->accept(TokenType::ELSE)) n->otherwise = this->block();

        return n;
    } else if (this->accept(TokenType::WHILE)) {
        LoopNode* n = new LoopNode;

        this->expect(TokenType::OPEN_PAREN, "While loop needs condition");
        n->condition = this->expression();
        this->expect(TokenType::CLOSE_PAREN, "Closing parenthasis after condition");

        n->logic = this->block();

        return n;
    } else if (this->accept(TokenType::FOR)) {
    }
    return this->expression();
};

Node* Parser::block() {
    if (this->accept(TokenType::OPEN_CURLY)) {
        BlockNode* n = new BlockNode;

        while (this->curr->type() != TokenType::CLOSE_CURLY)
            n->statements.push_back(this->block());
        
        this->expect(TokenType::CLOSE_CURLY, "Block must close");
        return n;
    }
    return this->statement();
};
