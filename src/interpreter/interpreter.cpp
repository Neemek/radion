#include <iostream>
#include <cmath>
#include <functional>
#include <cxxabi.h>
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

using namespace std::string_literals;

Interpreter::Interpreter()
{
    this->symbols = new SymbolTable();

    registerStandardLibrary(this->symbols);
}

void Interpreter::exit(std::string exit_message) {
    this->exit(new RuntimeException(exit_message));
}

void Interpreter::exit(RuntimeException* exception) {
    throw exception;
}

std::any Interpreter::evaluate(Node *programNode)
{
    std::any ret;

    if (programNode == nullptr) return ret;

    switch (programNode->type)
    {
    case NodeType::Arithmetic:
        ret = this->evaluate_arithemtic((ArithmeticNode *)programNode); break;

    // Literals
    case NodeType::BooleanLiteral:
        ret = ((BooleanLiteralNode *)programNode)->boolean; break;
    case NodeType::IntLiteral:
        ret = ((IntLiteralNode *)programNode)->number; break;
    case NodeType::StringLiteral:
        ret = ((StringLiteralNode *)programNode)->string; break;
    case NodeType::NilLiteral:
        ret = NIL_VALUE; break;

    case NodeType::Block:
    {
        BlockNode *block = (BlockNode *)programNode;
        this->table_descend();
        for (int i = 0; i < block->statements.size(); i++)
        {
            Node *statement = block->statements.at(i);
            this->evaluate(statement);

            if (this->returned.has_value()) { 
                if (block->isCapturing) {
                    ret = this->returned;
                    this->returned = std::any();
                }
                break;
            } else continue;
        }
        this->table_ascend();
    }
    break;

    case NodeType::Assign:
    {
        AssignNode *assign = (AssignNode *)programNode;
        std::any value = this->evaluate(assign->value);

        this->symbols->put(assign->name, value);
        ret = value;
    }
    break;

    case NodeType::If:
    {
        IfNode *conditional = (IfNode *)programNode;
        if (this->evaluate_boolean(conditional->condition))
            this->evaluate(conditional->logic);
        else if (conditional->otherwise != nullptr)
            this->evaluate(conditional->otherwise);
    }
    break;

    case NodeType::Reference:
    {
        ReferenceNode *reference = (ReferenceNode *)programNode;
        ret = this->symbols->get(reference->name);
    }
    break;
    case NodeType::Define:
    {
        DefineNode *definition = (DefineNode *)programNode;
        this->symbols->put(definition->name, new DefinedCallable(definition->name.c_str(), definition->params, definition->logic));
    }
    break;
    case NodeType::InlineDef:
    {
        InlineDefNode *definition = (InlineDefNode *)programNode;
        ret = new DefinedCallable(definition->name.c_str(), definition->params, definition->logic);
    }
    break;

    case NodeType::Return:
    {
        ReturnNode* returning = (ReturnNode *)programNode;
        this->returned = this->evaluate(returning->value);
    }
    break;

    case NodeType::Comparison:
        ret = this->evaluate_boolean(programNode);
        break;
    case NodeType::Not:
        ret = !this->evaluate_boolean(programNode);
        break;

    case NodeType::Change:
    {
        ChangeNode *change = (ChangeNode *)programNode;

        if (!change->ret_new) ret = this->symbols->get(change->name);
        
        int newValue;

        std::any value = this->symbols->get(change->name);
        if (value.type() == typeid(int)) newValue = std::any_cast<int>(value);
        else this->exit("variable " + change->name + ", value " + value_to_string(value) + " is not an integer");

        newValue += change->changeBy;

        this->symbols->put(change->name, newValue);

        if (change->ret_new) ret = newValue;
    }
    break;

    case NodeType::Call:
    {
        CallNode *call = (CallNode *)programNode;
        std::any func = this->symbols->get(call->name);

        if (!func.has_value()) {
            this->exit("invalid function: "+call->name);
        }

        Callable* callable = get_callable(func);
        
        std::vector<std::any> calledArgs;

        for (Node* node : call->params)
        {
            calledArgs.push_back(this->evaluate(node));
        }

        ret = callable->call(this, calledArgs);
    }
    break;
    case NodeType::Loop:
    {
        LoopNode *loop = (LoopNode *)programNode;

        this->nofree = true;
        while (this->evaluate_boolean(loop->condition))
        {
            this->evaluate(loop->logic);
            if (loop->doo != nullptr) this->evaluate(loop->doo);
        }
        this->nofree = false;
    }

    case NodeType::Range:
    {
        // Create range
        RangeNode *range = (RangeNode *)programNode;
        std::vector<int> r;

        int from = this->expect_any<int>(this->evaluate(range->from));
        int to = this->expect_any<int>(this->evaluate(range->to));
        int increase = this->expect_any<int>(this->evaluate(range->increase));

        while (increase < 0 ? from > to : from < to) r.push_back(from = from + increase);

        ret = r;
    }
    break;
    case NodeType::For:
    {
        ForNode *forloop = (ForNode *)programNode;

        std::any things = this->evaluate(forloop->values);
        if (things.type() == typeid(std::vector<std::any>)) {
            std::vector<std::any> values = std::any_cast<std::vector<std::any>>(things);

            this->nofree = true;
            for (std::any value : values) {
                this->symbols->put(forloop->counter, value);
                this->evaluate(forloop->logic);
            }
            this->nofree = false;
        } else {
            this->exit("Cannot iterate over value: " + value_to_string(things));
        }
        
    }
    break;
    default:
        ret = NIL_VALUE;
        break;
    }
    // clean up node, unless specified
    if (!this->nofree) delete programNode;
    
    return ret;
}

std::any Interpreter::evaluate_arithemtic(ArithmeticNode *arithmeticNode)
{
    std::any left = this->evaluate(arithmeticNode->left);
    std::any right = this->evaluate(arithmeticNode->right);

    if (!left.has_value() || !right.has_value())
    {
        this->exit("Missing value");
    }

    int a;
    int b;

    if (left.type() == typeid(int)) a = std::any_cast<int>(left);
    else if (left.type() == typeid(bool)) a = std::any_cast<bool>(left) ? 1 : 0;
    else if (left.type() == typeid(std::string)) return value_to_string(left) + value_to_string(right);
    else this->exit("invalid value for left side of arithmetic: " + value_to_string(left) + " " + left.type().name());
    
    if (right.type() == typeid(int)) b = std::any_cast<int>(right);
    else if (right.type() == typeid(bool)) b = std::any_cast<bool>(right) ? 1 : 0;
    else if (right.type() == typeid(std::string)) return std::to_string(a) + value_to_string(right);
    else this->exit("invalid value for right side of arithmetic: " + value_to_string(left) + " " + left.type().name());

    switch (arithmeticNode->op)
    {
    case ArithmeticOperation::ADD:
        return a + b;
    case ArithmeticOperation::SUBTRACT:
        return a - b;
    case ArithmeticOperation::MULTIPLY:
        return a * b;
    case ArithmeticOperation::DIVIDE:
        return a / b;
    case ArithmeticOperation::MODULO:
        return a % b;
    case ArithmeticOperation::EXPONENTIATION:
        return std::pow(a, b);

    default:
        return 0;
    }
}

int Interpreter::to_int(std::any v) {
    if (v.type() == typeid(int)) return std::any_cast<int>(v);
    else if (v.type() == typeid(bool)) return std::any_cast<bool>(v) ? 1 : 0;
    else this->exit("invalid value for right side of arithemtic");
    return 0;
}

bool Interpreter::evaluate_boolean(Node* expression) {
    switch (expression->type)
    {
    case NodeType::Comparison:
    {
        ComparisonNode* comparison = (ComparisonNode*)expression;
        std::any a = this->evaluate(comparison->left);
        std::any b = this->evaluate(comparison->right);

        switch (comparison->comparison)
        {
        case ComparisonType::Equals:
            return any_equals(a, b);
        case ComparisonType::NotEquals: 
            return !any_equals(a, b);

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
        std::any value = this->evaluate(expression);

        if (value.type() == typeid(int)) {
            return std::any_cast<int>(value) != 0;
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

template <typename T>
T Interpreter::expect_any(std::any v) {
    if (v.type() == typeid(T)) return std::any_cast<T>(v);
    else this->exit("Expected type to be "s + get_typename(typeid(T)) + ", got " + get_typename(v.type()));
}

std::string value_to_string(std::any value) {
    if (!value.has_value()) {
        return "nil";
    } else if (value.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(value));
    } else if (value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(value);
    } else if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value) ? "true" : "false";
    } else {
        try {
            auto callable = get_callable(value);
            return "<Function name="+std::string(callable->name)+">";
        } catch (const RuntimeException* e) {}
    }
    throw new RuntimeException("Unimplented value to string conversion for type "s+get_typename(value.type()));
    return "";
}

std::string get_typename(const std::type_info &t) {
    int status;
    std::string name = t.name();
    char *demangled  = abi::__cxa_demangle(name.c_str(), NULL, NULL, &status);
    if (status == 0) {
        name = demangled;
        std::free(demangled);
    }
    return name;
}

bool any_equals(std::any a, std::any b) {
    // They cannot have the samve value if they do not have the same type. 
    if (a.type() != b.type()) return false;
    if (!a.has_value()) return true; // since they have the same type, if one has no value then both have no value.

    if (a.type() == typeid(bool)) {
        bool va = std::any_cast<bool>(a);
        bool vb = std::any_cast<bool>(b);

        return va == vb;
    } else if (a.type() == typeid(int)) {
        int va = std::any_cast<int>(a);
        int vb = std::any_cast<int>(b);

        return va == vb;
    } else if (a.type() == typeid(std::string)) {
        std::string va = std::any_cast<std::string>(a);
        std::string vb = std::any_cast<std::string>(b);

        return va.compare(vb) == 0;
    } // else if (a.type() == typeid())
    throw RuntimeException("Uninplemented equality");
    return false;
}

bool cmp_any_int(std::any a, std::any b, std::function<bool(int, int)> comparison) {
    int ia;
    try {
        ia = std::any_cast<int>(a);
    } catch (const std::bad_any_cast& e) {
        throw new RuntimeException(value_to_string(a)+" is not comparable as an int");
    }

    int ib;
    try {
        ib = std::any_cast<int>(b);
    } catch (const std::bad_any_cast& e) {
        throw new RuntimeException(value_to_string(b)+" is not comparable as an int");
    }   
    
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
        LoopNode *loop = (LoopNode*) root;
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

