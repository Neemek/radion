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
#include <utility>
#include <cmath>

Parser::Parser() = default;

Parser::Parser(vector<Token> tokens, string src) {
    this->reset(std::move(tokens), std::move(src));
}

void Parser::reset(vector<Token> tokens, string src) {
    this->tokens = std::move(tokens);
    this->src = std::move(src);
    this->pos = 0;
    this->hadError = false;

    this->curr = &this->tokens.at(0);
}

BlockNode* Parser::parse() {
    auto* n = new BlockNode();

    n->start = this->start;
    while (this->curr->type() != TokenType::END)
        n->statements.push_back(this->block());
    n->end = this->prev_end;

    return n;
};

void Parser::nextToken() {
    if (this->pos+1 == this->tokens.size()) {
        throw ParsingException("Didnt stop at end of tokens!");
    }

    this->prev = curr;
    this->curr = &this->tokens[++this->pos];

    this->start = this->curr->pos();
    if (this->pos != 0) this->prev_end = this->prev->pos() + this->prev->lexeme().size();
}

std::string get_position_descriptor(std::string src, unsigned int pos) {
    int linen = 1;
    int from_start = 0;
    for (unsigned int i = 0; i < pos && i < src.length(); i++) {
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

bool Parser::accept_seq(std::vector<TokenType> ts) {
    for (unsigned int i = 0; i < ts.size(); ++i) {
        if (this->tokens.at(pos+i).type() != ts.at(i)) return false;
    }
    for (unsigned int i = 0; i < ts.size(); ++i) {
        this->nextToken();
    }
    return true;
};

void Parser::expect(TokenType type, const char* error) {
    if (!this->accept(type)) {
        this->error(this->curr, error);
        this->nextToken();
    }
};

Node* Parser::factor() {
    int start = this->start;
    if (this->accept(TokenType::NOT)) {
        auto* n = new NotNode;
        n->start = start;

        n->value = this->factor();
        n->end = this->prev_end;

        return n;
    } else if (this->accept(TokenType::STRING)) {
        auto* n = new StringLiteralNode;
        n->start = start;

        n->string = this->prev->lexeme().substr(1, this->prev->lexeme().length()-2);
        n->end = this->prev_end;

        return n;
    } else if (this->accept(TokenType::MINUS)) {
        auto* n = new NegationNode;

        n->value = this->factor();

        return n;
    } else if (this->accept(TokenType::NUMBER)) {
        int num = stoi(this->prev->lexeme());
        if (this->accept_seq(std::vector({ TokenType::DOT, TokenType::NUMBER }))) {
            auto *n = new DecimalLiteralNode;
            n->start = start;

            std::string decimal_part = this->prev->lexeme();
            double behind_decimal_point = 0;

            for (unsigned int i = 0; i < decimal_part.length(); i++) {
                char c = decimal_part.at(i);
                int digit = c - '0';

                behind_decimal_point += digit / std::pow(10, i+1);
            }

            n->number = (double)num + behind_decimal_point;

            n->end = this->prev_end;
            return n;
        } else {
            auto *n = new IntLiteralNode;
            n->start = start;

            n->number = num;
            n->end = this->prev_end;

            return n;
        }
    } else if (this->accept(TokenType::TRUE) || this->accept(TokenType::FALSE)) {
        auto* n = new BooleanLiteralNode;
        n->start = start;

        n->boolean = this->prev->type() == TokenType::TRUE;
        n->end = this->prev_end;

        return n;
    } else if (this->accept(TokenType::NIL)) {
        auto *n = new NilLiteralNode;

        n->start = start;
        n->end = this->prev_end;

        return n;
    } else if (this->accept(TokenType::OPEN_BRACKET)) {
        auto *n = new ListLiteralNode();
        n->start = start;

        if (this->accept(TokenType::CLOSE_BRACKET)) return n;

        do {
            n->elements.push_back(this->comparison());
        } while (this->accept(TokenType::COMMA));

        this->expect(TokenType::CLOSE_BRACKET, "list must have closing bracket");
        n->end = this->prev_end;

        return n;
    } else if (this->accept(TokenType::NAME)) {
        std::string name = this->prev->lexeme();
        // Differentiate between reference and function call
        if (this->accept(TokenType::OPEN_PAREN)) {
            auto* n = new CallNode;
            n->name = name;
            n->start = start;

            // if it doesnt close immediately
            if (!this->accept(TokenType::CLOSE_PAREN)) {
                // parse arguments
                int i = 0;
                do {
                    n->params.push_back(this->comparison());
                } while (++i < MAX_PARAMS && this->accept(TokenType::COMMA));

                this->expect(TokenType::CLOSE_PAREN, ("Maximum amount of parameters is "+to_string(MAX_PARAMS)).c_str());
            }
            n->end = this->prev_end;
            return n;
            
        } else if (this->accept(TokenType::ASSIGN)) {
            auto* n = new AssignNode;
            n->name = name;
            n->start = start;

            n->value = this->comparison();
            n->end = this->prev_end;

            return n;
        } else if (this->accept(TokenType::DOUBLE_PLUS) || this->accept(TokenType::DOUBLE_MINUS)) {
            auto* n = new ChangeNode;
            n->changeBy = this->prev->type() == TokenType::DOUBLE_PLUS ? 1 : -1;
            n->start = start;

            n->name = name;
            n->ret_new = false;
            n->end = this->prev_end;

            return n;
        } else {
            auto* n = new ReferenceNode;
            n->start = start;
            n->name = name;

            n->end = this->prev_end;
            return n;
        }
    } else if (this->accept(TokenType::DOUBLE_PLUS) || this->accept(TokenType::DOUBLE_MINUS)) {
        auto* n = new ChangeNode;
        n->changeBy = this->prev->type() == TokenType::DOUBLE_PLUS ? 1 : -1;
        n->start = start;

        n->ret_new = true;
        this->expect(TokenType::NAME, "Must increment variable");
        n->name = this->prev->lexeme();
        
        n->end = this->prev_end;
        return n;
    } else if (this->accept(TokenType::OPEN_PAREN)) {
        Node* n = this->comparison();
        n->start = start;
        this->expect(TokenType::CLOSE_PAREN, "Needs closing parenthasis");
        n->end = this->prev_end;
        return n;
    } else if (this->accept(TokenType::FUNC)) {
        // lambda basically
        auto* n = new InlineDefNode;
        n->start = start;

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

        n->logic = this->block(true);
        
        n->end = this->prev_end;
        return n;
    } else {
        this->error(this->curr, "factor: invalid token");
        this->nextToken();
        return new NilLiteralNode();
    }
};

Node* Parser::range() {
    int start = this->start;
    Node* f = this->factor();

    if (this->accept(TokenType::DOUBLE_DOT)) {
        auto* n = new RangeNode;
        n->start = start;

        // Get operation
        n->from = f;

        n->to = this->factor();

        if (this->accept(DOUBLE_DOT)) n->increase = this->factor();

        n->end = this->prev_end;
        return n;
    } else if (this->accept(TokenType::DOUBLE_STAR)) {
        // otherwise, create a mini tree
        ArithmeticNode* n = nullptr;
        do {
            auto* branch = new ArithmeticNode;
            branch->start = start;

            // Get operation
            branch->op = ArithmeticOperation::EXPONENTIATION;

            branch->right = this->factor();

            branch->end = this->prev_end;

            // if its the first branch, set the first factor as its left part
            if (n == nullptr) {
                branch->left = f;
            }
            // otherwise, "descend"
            else {
                branch->left = n;
            }
            // set the new branch as the root
            n = branch;
        } while (this->accept(TokenType::DOUBLE_STAR));

        return n;
    } else {
        return f;
    }
};

Node* Parser::product() {
    int start = this->start;
    Node* f = this->range();

    // if the next is NOT a valid operator
    if (!this->accept(TokenType::STAR)
        && !this->accept(TokenType::SLASH)
        && !this->accept(TokenType::DOUBLE_SLASH))
        return f;

    // otherwise, create a mini tree
    ArithmeticNode* n = nullptr;
    do {
        auto* branch = new ArithmeticNode;
        branch->start = start;

        // Get operation
        branch->op = this->prev->type() == TokenType::STAR
            ? ArithmeticOperation::MULTIPLY
            : this->prev->type() == TokenType::SLASH
                ? ArithmeticOperation::DIVIDE
                : ArithmeticOperation::INTEGER_DIVISION;

        branch->right = this->range();

        branch->end = this->prev_end;

        // if its the first branch, set the first factor as its left part
        if (n == nullptr) {
            branch->left = f;
        }
        // otherwise, "descend"
        else {
            branch->left = n;
        }
        // set the new branch as the root
        n = branch;
    } while (this->accept(TokenType::STAR) || this->accept(TokenType::SLASH) || this->accept(TokenType::DOUBLE_SLASH));

    return n;
};

Node* Parser::expression() {
    int start = this->start;
    Node* t = this->product();

    if (!this->accept(TokenType::PLUS) && !this->accept(TokenType::MINUS)) {
        return t;
    }

    ArithmeticNode* n = nullptr;
    do {
        auto* branch = new ArithmeticNode;
        branch->start = start;

        // Get operation
        branch->op = this->prev->type() == TokenType::PLUS
                     ? ArithmeticOperation::ADD
                     : ArithmeticOperation::SUBTRACT;

        branch->right = this->product();

        branch->end = this->prev_end;

        // if its the first branch, set the first factor as its left part
        if (n == nullptr) {
            branch->left = t;
        }
        // otherwise, "descend"
        else {
            branch->left = n;
        }
        // set the new branch as the root
        n = branch;
    } while (this->accept(TokenType::PLUS) || this->accept(TokenType::MINUS));

    return n;
}

Node* Parser::comparison() {
    int start = this->start;
    Node* t = this->expression();

    auto* n = new ComparisonNode;

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
    n->start = start;

    n->left = t;
    n->right = this->expression();
    n->end = this->prev_end;

    return n;
};

Node* Parser::statement() {
    int start = this->start;
    if (this->accept(TokenType::IF)) {
        auto* n = new IfNode;
        n->start = start;

        n->condition = this->comparison();
        n->logic = this->block();
        if (this->accept(TokenType::ELSE)) n->otherwise = this->block();

        n->end = this->prev_end;
        return n;
    } else if (this->accept(TokenType::WHILE)) {
        auto* n = new LoopNode;
        n->start = start;

        // Optional parenthasis :D
        bool needsClosing = this->accept(TokenType::OPEN_PAREN);
        n->condition = this->comparison();
        if (needsClosing) this->expect(TokenType::CLOSE_PAREN, "Closing parenthasis after condition");

        if (this->accept(TokenType::DO)) n->doo = this->expression();

        n->logic = this->block();

        n->end = this->prev_end;
        return n;
    } else if (this->accept(TokenType::FOR)) {
        auto* n = new ForNode;
        n->start = start;

        this->expect(TokenType::NAME, "required for-loop variable name");
        n->counter = this->prev->lexeme();

        this->expect(TokenType::IN, "for loop must iterate over something");

        n->values = this->range();

        n->logic = this->block();

        n->end = this->prev_end;
        return n;
    } else if (this->accept(TokenType::FUNC)) {
        auto* n = new DefineNode;
        n->start = start;
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

        n->logic = this->block(true);
        n->end = this->prev_end;
        return n;
    } else if (this->accept(TokenType::RETURN)) {
        auto *n = new ReturnNode;
        n->start = start;

        n->value = this->comparison();

        n->end = this->prev_end;
        return n;
    }
    return this->comparison();
};

Node* Parser::block(bool capturing) {
    int start = this->start;
    if (this->accept(TokenType::OPEN_CURLY)) {
        auto* n = new BlockNode;
        n->isCapturing = capturing;
        n->start = start;

        while (this->curr->type() != TokenType::CLOSE_CURLY)
            n->statements.push_back(this->block());
        
        this->expect(TokenType::CLOSE_CURLY, "Block must close");
        n->end = this->prev_end;
        return n;
    }
    return this->statement();
}

ParsingException::ParsingException(const char* message) {
    this->message = message;
}

const char *ParsingException::what() const noexcept {
    return this->message;
}