#include <iostream>
#include <cmath>
#include <functional>
#include "radion/interpreter/callable.hpp"
#include "radion/interpreter/interpreter.hpp"

#include "radion/parser/nodes/literal.hpp"
#include "radion/parser/nodes/reference.hpp"
#include "radion/parser/nodes/functions.hpp"
#include "radion/parser/nodes/conditionals.hpp"
#include "radion/parser/nodes/block.hpp"
#include "radion/parser/nodes/loops.hpp"

#define ERROR 'E' ^ 'R' ^ 'R' ^ 'O' ^ 'R'

Interpreter::Interpreter()
{
    // Native functions
    this->symbols["print"] = new NativeCallable("print", std::function([](std::any arguments[])
                                                              {
        auto out = value_to_string(arguments[0]);
        std::cout << out;

        return NIL_VALUE; }));
    this->symbols["println"] = new NativeCallable("println", std::function([](std::any arguments[])
                                                               {
        auto out = value_to_string(arguments[0]);
        std::cout << out << std::endl;

        return NIL_VALUE; }));
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
        for (int i = 0; i < block->statements.size(); i++)
        {
            Node *statement = block->statements.at(i);
            this->evaluate(statement);
        }
    }
    break;

    case NodeType::Assign:
    {
        AssignNode *assign = (AssignNode *)programNode;
        std::any value = this->evaluate(assign->value);

        this->symbols[assign->name] = value;
        ret = value;
    }
    break;

    case NodeType::If:
    {
        IfNode *conditional = (IfNode *)programNode;
        std::any condition = this->evaluate(conditional->condition);
        if ((condition.type() == typeid(bool) && std::any_cast<bool>(condition)) || (condition.type() == typeid(int) && std::any_cast<int>(condition) != 0))
            this->evaluate(conditional->logic);
        else
            this->evaluate(conditional->otherwise);
    }
    break;

    case NodeType::Reference:
    {
        ReferenceNode *reference = (ReferenceNode *)programNode;
        ret = this->symbols[reference->name];
    }
    break;
    case NodeType::Define:
    {
        DefineNode *definition = (DefineNode *)programNode;
        this->symbols[definition->name] = new DefinedCallable(definition->name.c_str(), definition->params, definition->logic);
    }
    break;

    case NodeType::Call:
    {
        CallNode *call = static_cast<CallNode*>(programNode);
        std::any func = this->symbols[call->name];
        Callable* callable = nullptr;

        if (!func.has_value()) {
            this->exit("invalid function: "+call->name);
        }

        try {
            callable = std::any_cast<NativeCallable*>(func);
        } catch (const std::bad_any_cast& _e) {}

        if (callable == nullptr) {
            try {
                callable = std::any_cast<DefinedCallable*>(func);
            } catch (const std::bad_any_cast& e) {
                this->exit("variable "+call->name+" is not a function");
            }
        }

        std::vector<std::any> calledArgs;

        for (int i = 0; i < MAX_PARAMS && call->params[i] != 0; i++)
        {
            calledArgs.push_back(this->evaluate(call->params[i]));
        }

        ret = callable->call(this, &calledArgs[0]);

    }
    break;
    case NodeType::Loop:
    {
        LoopNode *loop = (LoopNode *)programNode;
        while (std::any_cast<bool>(this->evaluate(loop->condition)))
        {
            this->evaluate(loop->logic);
        }
    }
    default:
        ret = NIL_VALUE;
        break;
    }
    // clean up node
    delete programNode;
    
    return ret;
}

int Interpreter::evaluate_arithemtic(ArithmeticNode *arithmeticNode)
{
    std::any left = this->evaluate(arithmeticNode->left);
    std::any right = this->evaluate(arithmeticNode->right);

    if (!left.has_value() || !right.has_value() || left.type() != typeid(int) || right.type() != typeid(int))
    {
        std::cout << "invalid arithemtic values" << std::endl;
        return ERROR;
    }

    int a = std::any_cast<int>(left);
    int b = std::any_cast<int>(right);

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
    case ArithmeticOperation::EXPONENTIATION:
        return std::pow(a, b);

    default:
        return 0;
    }
}

Callable* get_callable(std::any value) {
    Callable* callable;
    try {
        callable = std::any_cast<NativeCallable*>(value);
    } catch (const std::bad_any_cast& _e) {
        try {
            callable = std::any_cast<DefinedCallable*>(value);
        } catch (const std::bad_any_cast& e) {
            throw RuntimeException("value is not a callable");
        }
    }

    return callable;
}

std::string value_to_string(std::any value) {
    if (value.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(value));
    } else if (value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(value);
    } else if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value) ? "true" : "false";
    } else {
        try {
            auto callable = get_callable(value);
            return "<Function name="+std::string(callable->name)+">";
        } catch (const std::bad_any_cast& e) {}
    }
    return "";
}
