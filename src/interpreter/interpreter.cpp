#include <iostream>
#include <cmath>
#include <functional>
#include <utility>
#include "radion/interpreter/callable.hpp"
#include "radion/interpreter/interpreter.hpp"

#include "radion/interpreter/stlib/stlib.hpp"

#include "radion/parser/nodes/block.hpp"
#include "radion/parser/nodes/conditionals.hpp"
#include "radion/parser/nodes/comparison.hpp"
#include "radion/parser/nodes/functions.hpp"
#include "radion/parser/nodes/literal.hpp"
#include "radion/parser/nodes/loops.hpp"
#include "radion/parser/nodes/reference.hpp"

#include "radion/interpreter/values/constant.hpp"
#include "radion/interpreter/values/number.hpp"
#include "radion/interpreter/values/string.hpp"
#include "radion/interpreter/values/list.hpp"

using namespace std::string_literals;

Interpreter::Interpreter()
{
    this->symbols = new SymbolTable();

    registerStandardLibrary(this->symbols);
}

void Interpreter::exit(std::string exit_message) {
    Interpreter::exit(RuntimeException(std::move(exit_message), this->current_node));
}

void Interpreter::exit(RuntimeException exception) {
    throw exception;
}

Value* Interpreter::evaluate(Node *programNode)
{
    if (programNode == nullptr)
        this->exit("Node is a null-pointer");

    this->current_node = programNode;

    switch (programNode->type)
    {
    case NodeType::Arithmetic:
        return this->evaluate_arithemtic((ArithmeticNode *)programNode);

    // Literals
    case NodeType::BooleanLiteral:
        return ((BooleanLiteralNode *)programNode)->boolean ? BOOLEAN_TRUE : BOOLEAN_FALSE;
    case NodeType::IntLiteral:
        return new IntValue(((IntLiteralNode *)programNode)->number);
    case NodeType::StringLiteral:
        return new StringValue(((StringLiteralNode *)programNode)->string);
    case NodeType::NilLiteral:
        return NIL_VALUE;
    case NodeType::ListLiteral:
    {
        auto *node = (ListLiteralNode *)programNode;
        auto *list = new ListValue;

        for (Node *element : node->elements) {
            list->elements.push_back(this->evaluate(element));
        }

        return list;
    }

    case NodeType::Block:
    {
        auto *block = (BlockNode *)programNode;
        this->table_descend();
        for (auto *statement : block->statements)
        {
            this->evaluate(statement);

            if (this->returned != nullptr) {
                if (block->isCapturing) {
                    Value* ret = this->returned;
                    this->returned = nullptr;

                    return ret;
                }
                break;
            } else continue;
        }
        this->table_ascend();
    }
    break;

    case NodeType::Assign:
    {
        auto *assign = (AssignNode *)programNode;
        Value* value = this->evaluate(assign->value);

        this->symbols->put(assign->name, value);
        return value;
    }

    case NodeType::If:
    {
        auto *conditional = (IfNode *)programNode;
        if (this->evaluate_boolean(conditional->condition))
            this->evaluate(conditional->logic);
        else if (conditional->otherwise != nullptr)
            this->evaluate(conditional->otherwise);
    }
    break;

    case NodeType::Reference:
    {
        auto *reference = (ReferenceNode *)programNode;
        Value* value = this->symbols->get(reference->name);

        if (value == nullptr) this->exit("no variable of name \""+reference->name+"\"");
        return value;
    }
    case NodeType::Define:
    {
        auto *definition = (DefineNode *)programNode;
        this->symbols->put(definition->name, new DefinedCallable(definition->name, definition->params, definition->logic));
    }
    break;
    case NodeType::InlineDef:
    {
        auto *definition = (InlineDefNode *)programNode;
        return new DefinedCallable(definition->name, definition->params, definition->logic);
    }

    case NodeType::Return:
    {
        auto* returning = (ReturnNode *)programNode;
        this->returned = this->evaluate(returning->value);
    }
    break;

    case NodeType::Comparison:
        return new BooleanValue(this->evaluate_boolean(programNode));
    case NodeType::Not:
        return new BooleanValue(!this->evaluate_boolean(programNode));

    case NodeType::Change:
    {
        auto *change = (ChangeNode *)programNode;

        Value* val = this->symbols->get(change->name);
        auto* value = val->expect_type(ValueType::Int)->as<IntValue>();

        value->number += change->changeBy;

        return value;
    }

    case NodeType::Call:
    {
        auto *call = (CallNode *)programNode;
        Value *func = this->symbols->get(call->name);

        if (func == nullptr) {
            this->exit("undefined function: "+call->name);
        }

        if (func->get_type() != ValueType::Func) {
            this->exit("variable "+call->name+" is not a function (got type "+func->get_typename()+")");
        }

        auto *callable = (Callable *)func;

        std::vector<Value*> calledArgs;

        calledArgs.reserve(call->params.size());
        for (Node* node : call->params)
        {
            calledArgs.push_back(this->evaluate(node));
        }

        return callable->call(this, calledArgs);
    }

    case NodeType::Loop:
    {
        auto *loop = (LoopNode *)programNode;

        this->nofree++;
        while (this->evaluate_boolean(loop->condition))
        {
            this->evaluate(loop->logic);
            if (loop->doo != nullptr) this->evaluate(loop->doo);
        }
        this->nofree--;
    }
    break;

    case NodeType::Range:
    {
        // Create range
        auto *range = (RangeNode *)programNode;
        std::vector<Value*> r;

        auto *fromv = this->evaluate(range->from)->expect_type(ValueType::Int)->as<IntValue>();
        auto *tov = this->evaluate(range->to)->expect_type(ValueType::Int)->as<IntValue>();

        int increase;
        if (range->increase != nullptr) increase = this->evaluate(range->increase)->expect_type(ValueType::Int)->as<IntValue>()->number;
        else increase = 1;

        int from = fromv->number;
        int to = tov->number;

        while (increase < 0
                ? from > to
                : from < to) {
            r.push_back(new IntValue(from));
            from += increase;
        }

        return new ListValue(r);
    }

    case NodeType::For:
    {
        auto *forloop = (ForNode *)programNode;

        Value* things = this->evaluate(forloop->values);
        if (things->get_type() == ValueType::List) {
            auto values = things->as<ListValue>();

            this->nofree++;
            for (Value* value : values->elements) {
                this->symbols->put(forloop->counter, value);
                this->evaluate(forloop->logic);
            }
            this->nofree--;
        } else {
            this->exit("Cannot iterate over value: " + things->to_string());
        }
        break;
    }
    }
    // clean up node, unless specified
    if (!this->nofree) delete programNode;
    
    return NIL_VALUE;
}

Value* Interpreter::evaluate_arithemtic(ArithmeticNode *arithmeticNode)
{
    Value* left = this->evaluate(arithmeticNode->left);
    Value* right = this->evaluate(arithmeticNode->right);

    if (left == nullptr)
        this->exit("Missing value on left side of arithmetic");
    if (right == nullptr)
        this->exit("Missing value on right side of arithmetic");

    int a;
    int b;

    if (left->get_type() == ValueType::Int) a = left->as<IntValue>()->number;
    else if (left->get_type() == ValueType::Boolean) a = left->as<BooleanValue>()->boolean ? 1 : 0;
    else if (left->get_type() == ValueType::String && arithmeticNode->op == ArithmeticOperation::ADD) return new StringValue(left->to_string() + right->to_string());
    else this->exit("invalid value for left side of arithmetic: "+left->to_string()+" (type "+left->get_typename()+")");
    
    if (right->get_type() == ValueType::Int) b = right->as<IntValue>()->number;
    else if (right->get_type() == ValueType::Boolean) b = right->as<BooleanValue>()->boolean ? 1 : 0;
    else if (right->get_type() == ValueType::String && arithmeticNode->op == ArithmeticOperation::ADD) return new StringValue(left->to_string() + right->to_string());
    else this->exit("invalid value for right side of arithmetic: "+right->to_string()+" (type "+right->get_typename()+")");

    switch (arithmeticNode->op)
    {
    case ArithmeticOperation::ADD:
        return new IntValue(a + b);
    case ArithmeticOperation::SUBTRACT:
        return new IntValue(a - b);
    case ArithmeticOperation::MULTIPLY:
        return new IntValue(a * b);
    case ArithmeticOperation::DIVIDE:
        return new IntValue(a / b);
    case ArithmeticOperation::MODULO:
        return new IntValue(a % b);
    case ArithmeticOperation::EXPONENTIATION:
        return new IntValue(std::pow(a, b));

    default:
        this->exit("Unimplemented arithmetic operation (type "+std::to_string(arithmeticNode->op)+")");
    }
}

bool Interpreter::evaluate_boolean(Node* expression) {
    switch (expression->type)
    {
    case NodeType::Comparison:
    {
        auto* comparison = (ComparisonNode*)expression;
        Value *a = this->evaluate(comparison->left);
        Value *b = this->evaluate(comparison->right);

        switch (comparison->comparison)
        {
        case ComparisonType::Equals:
            return a->equals(b);
        case ComparisonType::NotEquals:
            return !a->equals(b);

        case ComparisonType::Greater:
            return cmp_any_int(a, b, [](int l, int r) { return l > r; });
        case ComparisonType::GreaterOrEqual:
            return cmp_any_int(a, b, [](int l, int r) { return l >= r; });
        case ComparisonType::Less:
            return cmp_any_int(a, b, [](int l, int r) { return l < r; });
        case ComparisonType::LessOrEqual:
            return cmp_any_int(a, b, [](int l, int r) { return l <= r; });

        default:
            break;
        }
        break;
    }
    default:
        Value* value = this->evaluate(expression);

        if (value->get_type() == ValueType::Int) {
            return value->as<IntValue>()->number != 0;
        }
        break;
    }

    this->exit("Unimplemented comparison");
    return false;
}

void Interpreter::table_descend() {
    this->symbols = this->symbols->sub();
}

void Interpreter::table_ascend() {
    SymbolTable* super = this->symbols->super;
    if (super == nullptr) this->exit("Cannot ascend further, at top.");
    delete this->symbols;
    this->symbols = super;
}

bool cmp_any_int(Value* a, Value* b, const std::function<bool(int, int)>& comparison) {
    int ia;
    if (a->get_type() == ValueType::Int)
        ia = a->as<IntValue>()->number;
    else
        throw RuntimeException(a->get_typename()+" is not comparable as an int");

    int ib;
    if (b->get_type() == ValueType::Int)
        ib = b->as<IntValue>()->number;
    else
        throw RuntimeException(b->get_typename()+" is not comparable as an int");

    return comparison(ia, ib);
}

void printAST(Node* root) {
    switch (root->type)
    {
    case NodeType::IntLiteral:
        std::cout << ((IntLiteralNode *)root)->number;
        break;
    case NodeType::StringLiteral:
        std::cout << "\"" << ((StringLiteralNode *)root)->string << "\"";
        break;
    case NodeType::BooleanLiteral:
        std::cout << (((BooleanLiteralNode *)root)->boolean ? "true" : "false");
        break;
    case NodeType::NilLiteral:
        std::cout << "nil";
        break;
    
    case NodeType::Reference:
        std::cout << "reference: " << ((ReferenceNode *)root)->name;
        break;
    case NodeType::Assign:
        std::cout << "assign: " << ((AssignNode *)root)->name;
        printAST(((AssignNode *)root)->value);
        std::cout << std::endl;
        break;
    case NodeType::Loop:
    {
        auto *loop = (LoopNode*) root;
        std::cout << "-- loop --" << std::endl;
        std::cout << "condition: ";
        printAST(loop->condition);
        if (loop->doo != nullptr) {
            std::cout << std::endl << "doo: ";
            printAST(loop->doo);
        }
        std::cout << "body" << std::endl;
        printAST(loop->logic);
        std::cout << std::endl << std::endl;
    }
    
    default:
        break;
    }
}

