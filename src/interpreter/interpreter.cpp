#include <iostream>
#include <cmath>
#include <functional>
#include "radion/interpreter/callable.hpp"
#include "radion/interpreter/interpreter.hpp"

#include "radion/parser/nodes/arithmetic.hpp"
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
    this->symbols["print"] = new NativeCallable(std::function([](std::any arguments[])
                                                              {
        auto out = std::any_cast<std::string>(arguments[0]);
        std::cout << out;

        return NIL_VALUE; }));
    this->symbols["println"] = new NativeCallable(std::function([](std::any arguments[])
                                                               {
        auto out = std::any_cast<std::string>(arguments[0]);
        std::cout << out << std::endl;

        return NIL_VALUE; }));
}

std::any Interpreter::evaluate(Node *program)
{
    std::any ret;

    switch (program->type)
    {
    case NodeType::Arithmetic:
        ret = this->evaluate_arithemtic((ArithmeticNode *)program); break;

    // Literals
    case NodeType::BooleanLiteral:
        ret = ((BooleanLiteralNode *)program)->boolean; break;
    case NodeType::IntLiteral:
        ret = ((IntLiteralNode *)program)->number; break;
    case NodeType::StringLiteral:
        ret = ((StringLiteralNode *)program)->string; break;
    case NodeType::NilLiteral:
        ret = NIL_VALUE; break;

    case NodeType::Block:
    {
        BlockNode *block = (BlockNode *)program;
        for (int i = 0; i < block->statements.size(); i++)
        {
            Node *statement = block->statements.at(i);
            this->evaluate(statement);
        }
    }
    break;

    case NodeType::If:
    {
        IfNode *conditional = (IfNode *)program;
        std::any condition = this->evaluate(conditional->condition);
        if ((condition.type() == typeid(bool) && std::any_cast<bool>(condition)) || (condition.type() == typeid(int) && std::any_cast<int>(condition) != 0))
            this->evaluate(conditional->logic);
        else
            this->evaluate(conditional->otherwise);
    }
    break;

    case NodeType::Reference:
    {
        ReferenceNode *reference = (ReferenceNode *)program;
        ret = this->symbols[reference->name];
    }
    break;
    case NodeType::Define:
    {
        DefineNode *definition = (DefineNode *)program;
        this->symbols[definition->name] = new DefinedCallable(definition->params, definition->logic);
    }
    break;

    case NodeType::Call:
    {
        CallNode *call = static_cast<CallNode*>(program);
        std::any func = this->symbols[call->name];

        try {
            NativeCallable* callable = std::any_cast<NativeCallable*>(func);
            std::vector<std::any> calledArgs;

            for (int i = 0; i < MAX_PARAMS && call->params[i] != 0; i++)
            {
                calledArgs.push_back(this->evaluate(call->params[i]));
            }

            ret = callable->call(this, &calledArgs[0]);
        } catch (const std::bad_any_cast& e) {
            // error "func is not callable"
            std::cout << "func " << call->name << " is not callable" << std::endl;
            ret = ERROR;
        }

    }
    break;
    case NodeType::Loop:
    {
        LoopNode *loop = (LoopNode *)program;
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
    //delete program;
    
    return ret;
}

int Interpreter::evaluate_arithemtic(ArithmeticNode *program)
{
    std::any left = this->evaluate(program->left);
    std::any right = this->evaluate(program->right);

    if (!left.has_value() || !right.has_value() || left.type() != typeid(int) || right.type() != typeid(int))
    {
        std::cout << "invalid arithemtic values" << std::endl;
        return ERROR;
    }

    int a = std::any_cast<int>(left);
    int b = std::any_cast<int>(right);

    switch (program->op)
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
