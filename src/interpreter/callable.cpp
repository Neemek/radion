#include "radion/interpreter/callable.hpp"
#include <any>
#include <algorithm>
#include <iterator>
#include <utility>

std::string Callable::to_string() {
    return this->name;
}

DefinedCallable::DefinedCallable(DefinedCallable* callable) : Callable() {
    this->name = callable->name;
    this->arguments = callable->arguments;
    this->logic = callable->logic;
}

DefinedCallable::DefinedCallable(const char* name, std::vector<std::string> arguments, Node* logic) : Callable() {
    this->name = name;
    this->arguments = std::move(arguments);
    this->logic = logic;
}

Value* DefinedCallable::call(Interpreter* interpreter, std::vector<Value*> arguments) {
    interpreter->table_descend();
    for (int i = 0; i < arguments.size(); i++)
    {
        interpreter->symbols->put(this->arguments.at(i), arguments.at(i));
    }
    

    interpreter->nofree = true;
    Value* ret = interpreter->evaluate(this->logic);
    interpreter->nofree = false;

    interpreter->table_ascend();
    return ret;
}

bool DefinedCallable::equals(Value *other) {
    return other->has_type(ValueType::Func) && this->name == other->as<Callable>()->name;
}


NativeCallable::NativeCallable(const char* name, std::function<Value*(std::vector<Value*> arguments)> function) : Callable() {
    this->name = name;
    this->logic = std::move(function);
}

Value* NativeCallable::call(Interpreter* interpreter, std::vector<Value*> arguments) {
    return this->logic(arguments);
}

bool NativeCallable::equals(Value *other) {
    return other->has_type(ValueType::Func) && this->name == other->as<Callable>()->name;
}
