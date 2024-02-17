#include "radion/parser/parser.hpp"
#include "radion/parser/node.hpp"
#include "radion/token.hpp"

#include "radion/parser/nodes/arithmetic.hpp"
#include "radion/parser/nodes/block.hpp"
#include "radion/parser/nodes/conditionals.hpp"
#include "radion/parser/nodes/comparison.hpp"
#include "radion/parser/nodes/functions.hpp"
#include "radion/parser/nodes/literal.hpp"
#include "radion/parser/nodes/loops.hpp"
#include "radion/parser/nodes/reference.hpp"


#include <iostream>

Parser::Parser() {}

Parser::Parser(vector<Token> tokens, string src) {
    this->reset(tokens, src);
}

void Parser::reset(vector<Token> tokens, string src) {
    this->tokens = tokens;
    this->src = src;
    this->pos = -1;

    this->nextToken();
}

BlockNode* Parser::parse() {
    BlockNode* n = new BlockNode();

    while (this->curr->type() != TokenType::END)
        n->statements.push_back(this->block());
    
    return n;
};

void Parser::nextToken() {
    if (this->pos+1 == this->tokens.size()) {
        throw "Didnt stop at end of tokens!";
    }

    this->prev = curr;
    this->curr = &this->tokens[++this->pos];
}

std::string get_position_descriptor(std::string src, int pos) {
    int linen = 1;
    int from_start = 0;
    for (int i=0; i < pos && i < src.length(); i++) {
        if (src.at(i) == '\n') {
            linen++;
            from_start = 0;
        }
        from_start++;
    }
    return to_string(linen)+":"+to_string(from_start);
}

void Parser::error(Token* token, const char* message) {
    // do error handling
    
    std::cout << get_position_descriptor(this->src, token->pos()) 
        << " " << ttype_to_string(token->type()) << " => \"" << token->lexeme() 
        << "\" & " << message << std::endl;
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
        this->error(this->curr, error);
        this->nextToken();
    }
};

Node* Parser::factor() {
    if (this->accept(TokenType::NOT)) {
        NotNode* n = new NotNode;

        n->value = this->factor();
        
        return n;
    } else if (this->accept(TokenType::STRING)) {
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

            // if closes immediatly, return with no arguments
            if (this->accept(TokenType::CLOSE_PAREN)) return n;
            
            // otherwise, parse arguments 
            int i = 0;
            do {
                n->params.push_back(this->expression());
            } while (++i < MAX_PARAMS && this->accept(TokenType::COMMA));

            this->expect(TokenType::CLOSE_PAREN, ("Maximum amount of parameters is "+to_string(MAX_PARAMS)).c_str());
            return n;
            
        } else if (this->accept(TokenType::ASSIGN)) {
            AssignNode* n = new AssignNode;
            n->name = name;

            n->value = this->expression();

            return n;
        } else if (this->accept(TokenType::DOUBLE_PLUS) || this->accept(TokenType::DOUBLE_MINUS)) {
            ChangeNode* n = new ChangeNode;
            n->changeBy = this->prev->type() == TokenType::DOUBLE_PLUS ? 1 : -1;

            n->name = name;
            n->ret_new = false;

            return n;
        } else {
            ReferenceNode* n = new ReferenceNode;
            n->name = name;

            return n;
        }
    } else if (this->accept(TokenType::DOUBLE_PLUS) || this->accept(TokenType::DOUBLE_MINUS)) {
        ChangeNode* n = new ChangeNode;
        n->changeBy = this->prev->type() == TokenType::DOUBLE_PLUS ? 1 : -1;
        
        n->ret_new = true;
        this->expect(TokenType::NAME, "Must increment variable");
        n->name = this->prev->lexeme();
        
        return n;
    } else if (this->accept(TokenType::OPEN_PAREN)) {
        Node* n = this->expression();
        this->expect(TokenType::CLOSE_PAREN, "Needs closing parenthasis");
        return n;
    } else if (this->accept(TokenType::FUNC)) {
        // lambda basically
        InlineDefNode* n = new InlineDefNode;

        std::string name = "*";
        if (this->accept(TokenType::NAME)) name = this->prev->lexeme();

        n->name = name;

        if (this->accept(TokenType::OPEN_PAREN) && !this->accept(TokenType::CLOSE_PAREN)){
            do {
                this->expect(TokenType::NAME, "Expected parameter name");
                n->params.push_back(this->prev->lexeme());
            } while (this->accept(TokenType::COMMA));
            this->expect(TokenType::CLOSE_PAREN, "Needs closing parenthasis");
        }

        n->logic = this->block();
        
        return n;
    } else {
        this->error(this->curr, "factor: invalid token");
        this->nextToken();
        return new NilLiteralNode();
    }
};

Node* Parser::range() {
    Node* f = this->factor();

    if (this->accept(TokenType::DOUBLE_DOT)) {
        RangeNode* n = new RangeNode;

        // Get operation
        n->from = f;

        n->to = this->factor();

        if (this->accept(DOUBLE_DOT)) n->increase = this->factor();

        return n;
    } else {
        return f;
    }
};

Node* Parser::term() {
    Node* f = this->range();

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

Node* Parser::product() {
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
}

Node* Parser::expression() {
    Node* t = this->product();

    ComparisonNode* n = new ComparisonNode;

    if (this->accept(TokenType::EQUAL)) n->comparison = ComparisonType::Equals;
    else if (this->accept(TokenType::NOT_EQUAL)) n->comparison = ComparisonType::NotEquals;
    else if (this->accept(TokenType::GREATER_THAN)) n->comparison = ComparisonType::Greater;
    else if (this->accept(TokenType::GREATER_EQUAL)) n->comparison = ComparisonType::GreaterOrEqual;
    else if (this->accept(TokenType::LESS_THAN)) n->comparison = ComparisonType::Less;
    else if (this->accept(TokenType::LESS_EQUAL)) n->comparison = ComparisonType::LessOrEqual;
    else {
        delete n;
        return t;
    }

    n->left = t;
    n->right = this->expression();

    return n;
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

        // Optional parenthasis :D
        bool needsClosing = this->accept(TokenType::OPEN_PAREN);
        n->condition = this->expression();
        if (needsClosing) this->expect(TokenType::CLOSE_PAREN, "Closing parenthasis after condition");

        if (this->accept(TokenType::DO)) n->doo = this->expression();

        n->logic = this->block();

        return n;
    } else if (this->accept(TokenType::FOR)) {
        ForNode* n = new ForNode;

        return n;
    } else if (this->accept(TokenType::FUNC)) {
        DefineNode* n = new DefineNode;
        this->expect(TokenType::NAME, "Invalid function name");

        n->name = this->prev->lexeme();

        // accept no parenthasis? why not :)
        // if theres an open paren but not a closing parenthasis immediatly following
        if (this->accept(TokenType::OPEN_PAREN) && !this->accept(TokenType::CLOSE_PAREN)) {
            int i = 0;
            do {
                this->expect(TokenType::NAME, "invalid argument name");
                n->params.push_back(this->prev->lexeme());
            } while (++i < MAX_PARAMS && this->accept(TokenType::COMMA));

            this->expect(TokenType::CLOSE_PAREN, ("Maximum amount of parameters is "+to_string(MAX_PARAMS)).c_str());
        }

        n->logic = this->block();
        return n;
    } else if (this->accept(TokenType::RETURN)) {
        
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
