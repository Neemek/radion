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
    if (programNode == nullptr) {
        this->exit("Node is a null-pointer");
        return nullptr;
    }

    this->current_node = programNode;

    switch (programNode->type)
    {
    case NodeType::Arithmetic:
        return this->evaluate_arithemtic((ArithmeticNode *)programNode);
    case NodeType::Negation:
    {
        auto *negation = (NegationNode *) programNode;
        Value *v = this->evaluate(negation->value);

        if (v->has_type(ValueType::Int)) return new IntValue(-v->as<IntValue>()->number);
        else if (v->has_type(ValueType::Decimal)) return new DecimalValue(-v->as<DecimalValue>()->number);
        else this->exit("Cannot negate value " + v->to_string() + " of type " + v->get_typename());
    }
    break;

    // Literals
    case NodeType::BooleanLiteral:
        return ((BooleanLiteralNode *)programNode)->boolean ? BOOLEAN_TRUE : BOOLEAN_FALSE;
    case NodeType::IntLiteral:
        return new IntValue(((IntLiteralNode *)programNode)->number);
    case NodeType::DecimalLiteral:
        return new DecimalValue(((DecimalLiteralNode *)programNode)->number);
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
                if (!block->isCapturing) break;

                Value *ret = this->returned;
                this->returned = nullptr;

                return ret;
            }
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
        return BooleanValue::from(this->evaluate_boolean(programNode));
    case NodeType::Not:
        return BooleanValue::from(!this->evaluate_boolean(programNode));

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

    double a;
    double b;

    ValueType returnValue = ValueType::Int;

    switch (left->get_type()) {
        case Int:
            a = left->as<IntValue>()->number;
            break;
        case Decimal:
            a = left->as<DecimalValue>()->number;
            returnValue = ValueType::Decimal;
            break;
        case Boolean:
            a = left->as<BooleanValue>()->boolean ? 1 : 0;
            break;
        case String:
            if (arithmeticNode->op == ArithmeticOperation::ADD)
                return new StringValue(left->to_string() + right->to_string());
            this->exit("invalid string operation: "+operation_to_symbol(arithmeticNode->op));
            break;
        default:
            this->exit("invalid value for right side of arithmetic: "+right->to_string()+" (type "+right->get_typename()+")");
            break;
    }

    switch (right->get_type()) {
        case Int:
            b = right->as<IntValue>()->number;
            break;
        case Decimal:
            b = right->as<DecimalValue>()->number;
            returnValue = ValueType::Decimal;
            break;
        case Boolean:
            b = right->as<BooleanValue>()->boolean ? 1 : 0;
            break;
        case String:
            if (arithmeticNode->op == ArithmeticOperation::ADD)
                return new StringValue(left->to_string() + right->to_string());
            this->exit("invalid operation: "+operation_to_symbol(arithmeticNode->op));
            break;
        default:
            this->exit("invalid value for right side of arithmetic: "+right->to_string()+" (type "+right->get_typename()+")");
            break;
    }

    double result;

    switch (arithmeticNode->op)
    {
    case ArithmeticOperation::ADD:
        result = a + b;
        break;
    case ArithmeticOperation::SUBTRACT:
        result = a - b;
        break;
    case ArithmeticOperation::MULTIPLY:
        result = a * b;
        break;
    case ArithmeticOperation::DIVIDE:
        result = a / b;
        if (returnValue != ValueType::Decimal && fmod(a, b) != 0.0f) returnValue = ValueType::Decimal;
        break;
    case ArithmeticOperation::INTEGER_DIVISION:
        result = a / b;
        returnValue = ValueType::Int;
        break;
    case ArithmeticOperation::MODULO:
        result = fmod(a, b);
        break;
    case ArithmeticOperation::EXPONENTIATION:
        result = std::pow(a, b);
        break;

    default:
        this->exit("Unimplemented arithmetic operation (type "+std::to_string(arithmeticNode->op)+")");
    }

    if (returnValue == ValueType::Decimal) return new DecimalValue(result);
    return new IntValue((int)result);
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
            return cmp_any_num(a, b, [](int l, int r) { return l > r; });
        case ComparisonType::GreaterOrEqual:
            return cmp_any_num(a, b, [](int l, int r) { return l >= r; });
        case ComparisonType::Less:
            return cmp_any_num(a, b, [](int l, int r) { return l < r; });
        case ComparisonType::LessOrEqual:
            return cmp_any_num(a, b, [](int l, int r) { return l <= r; });

        default:
            break;
        }
        break;
    }
    default:
        Value* value = this->evaluate(expression);

        switch (value->get_type()) {
            case ValueType::Int:
                return value->as<IntValue>()->number != 0;
            case ValueType::Decimal:
                return value->as<DecimalValue>()->number != 0.0f;
            default:
                break;
        }
        break;
    }

    this->exit("Unimplemented comparison");
    return false;
}

std::string operation_to_symbol(ArithmeticOperation operation) {
    switch (operation) {
        case ADD:
            return "+";
        case SUBTRACT:
            return "-";
        case MULTIPLY:
            return "*";
        case DIVIDE:
            return "/";
        case INTEGER_DIVISION:
            return "//";
        case MODULO:
            return "%";
        case EXPONENTIATION:
            return "**";
    }
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

bool cmp_any_num(Value* a, Value* b, const std::function<bool(double, double)>& compare) {
    double ia;
    switch (a->get_type()) {
        case Int:
            ia = a->as<IntValue>()->number;
            break;
        case Decimal:
            ia = a->as<DecimalValue>()->number;
            break;
        default:
            throw RuntimeException(a->get_typename()+" is not comparable as a number");
    }

    double ib;
    switch (b->get_type()) {
        case Int:
            ib = b->as<IntValue>()->number;
            break;
        case Decimal:
            ib = b->as<DecimalValue>()->number;
            break;
        default:
            throw RuntimeException(b->get_typename()+" is not comparable as a number");
    }

    return compare(ia, ib);
}

int DEPHT_LEVEL = -1;

void newLineAST() {
    std::cout << std::endl;
    for (int i = 0; i < DEPHT_LEVEL; i++) std::cout << "| ";
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
        std::cout << "value of " << ((ReferenceNode *)root)->name;
        break;
    case NodeType::Assign:
        std::cout << "assign ";
        printAST(((AssignNode *)root)->value);
        std::cout << " to variable " << ((AssignNode *)root)->name;
        newLineAST();
        break;
    case NodeType::Loop:
    {
        auto *loop = (LoopNode*) root;
        std::cout << "while ";
        printAST(loop->condition);
        if (loop->doo != nullptr) {
            std::cout << "(per iteration do ";
            printAST(loop->doo);
            std::cout << ") ";
        }
        std::cout << "do ";
        printAST(loop->logic);
    }
    case DecimalLiteral:
        std::cout << std::to_string(((DecimalLiteralNode *)root)->number);
        break;
    case ListLiteral:
    {
        auto *list = (ListLiteralNode *) root;
        std::cout << "[";

        for (int i = 0; i < list->elements.size(); i++) {
            Node *element = list->elements.at(i);
            printAST(element);
            if (i + 1 < list->elements.size()) std::cout << ", ";
        }

        std::cout << "]";
    }
    break;
    case For:
    {
        auto *loop = (ForNode *) root;
        std::cout << "for each in ";
        printAST(loop->values);
        std::cout << " stored in variable " << loop->counter << " do ";
        printAST(loop->logic);
    }
    break;
    case Change:
    {
        auto *change = (ChangeNode *) root;
        std::cout << "change variable " << change->name << " by " << change->changeBy;
        if (change->ret_new) std::cout << " and yield new value";
    }
    break;
    case Arithmetic:
    {
        auto *arithmetic = (ArithmeticNode *) root;
        printAST(arithmetic->left);
        std::cout << " " << operation_to_symbol(arithmetic->op) << " ";
        printAST(arithmetic->right);
    }
    break;
    case Comparison:
    {
        auto *comparison = (ComparisonNode *)root;
        printAST(comparison->left);
        switch (comparison->comparison) {

            case Equals:
                std::cout << " is equal to ";
                break;
            case NotEquals:
                std::cout << " isn't equal to ";
                break;
            case Greater:
                std::cout << " is greater than ";
                break;
            case GreaterOrEqual:
                std::cout << " is greater than or equal ";
                break;
            case Less:
                std::cout << " is less than ";
                break;
            case LessOrEqual:
                std::cout << " is less than or equal ";
                break;
        }
        printAST(comparison->right);
    }
    break;
    case Negation:
        std::cout << "negate ";
        printAST(((NegationNode *)root)->value);
    break;
    case Not:
        std::cout << "not ";
        printAST(((NotNode *)root)->value);
    break;
    case If:
    {
        auto *iffie = (IfNode *) root;
        std::cout << "if ";
        printAST(iffie->condition);
        std::cout << " then ";
        printAST(iffie->logic);

        if (iffie->otherwise != nullptr) {
            std::cout << "otherwise ";
            printAST(iffie->otherwise);
        }
    }
    break;
    case Define:
    {
        auto *definition = (DefineNode *)root;
        std::cout << "define function " << definition->name;
        if (definition->params.size() > 0) {
            std::cout << " and take params ";

            for (std::string param : definition->params) {
                std::cout << param << " ";
            }
        }

        std::cout << ", then ";
        printAST(definition->logic);
    }
    break;
    case InlineDef:
    {
        auto *lambda = (InlineDefNode *) root;
        std::cout << "lambda ";

        if (lambda->params.size() > 0) {
            std::cout << "with params ";

            std::cout << "[";

            for (int i = 0; i < lambda->params.size(); i++) {
                std::cout << lambda->params.at(i);
                if (i+1 < lambda->params.size()) std::cout << ", ";
            }

            std::cout << "] ";
        }

        printAST(lambda->logic);
    }
    break;
    case Return:
    {
        std::cout << "return ";
        printAST(((ReturnNode *)root)->value);
    }
    break;
    case Call:
    {
        auto *call = (CallNode *)root;
        std::cout << "call function \"" << call->name << "\"";

        if (call->params.size() > 0) {
            std::cout << " with params [";

            for (int i = 0; i < call->params.size(); i++) {
                printAST(call->params.at(i));
                if (i+1 < call->params.size()) std::cout << ", ";
            }

            std::cout << "]";
        }

    }
    break;
    case Range:
    {
        auto *range = (RangeNode *)root;
        std::cout
            << "range from " << range->from
            << " to " << range->to
            << " (incremented by " << range->increase << ")";
    }
    break;
    case Block:
    {
        auto *block = (BlockNode *)root;

        DEPHT_LEVEL++;
        for (Node *statement : block->statements) {
            newLineAST();
            printAST(statement);
        }
        DEPHT_LEVEL--;
    }
    break;
    }
}

