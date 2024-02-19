#include "radion/interpreter/callable.hpp"
#include <algorithm>
#include <iterator>
#include <utility>

std::string Callable::to_string() {
    return "<Function name="+std::string(this->name)+">";
}

DefinedCallable::DefinedCallable(DefinedCallable* callable) : Callable() {
    this->name = callable->name;
    this->arguments = callable->arguments;
    this->logic = callable->logic;
}

DefinedCallable::DefinedCallable(std::string name, std::vector<std::string> arguments, Node* logic) : Callable() {
    this->name = std::move(name);
    this->arguments = std::move(arguments);
    this->logic = logic;
}

Value* DefinedCallable::call(Interpreter* interpreter, std::vector<Value*> arguments) {
    interpreter->table_descend();
    for (int i = 0; i < arguments.size(); i++)
    {
        interpreter->symbols->put(this->arguments.at(i), arguments.at(i));
    }
    

    interpreter->nofree++;
    Value* ret = interpreter->evaluate(this->logic);
    interpreter->nofree--;

    interpreter->table_ascend();
    return ret;
}

bool DefinedCallable::equals(Value *other) {
    return other->has_type(ValueType::Func) && this->name == other->as<Callable>()->name;
}


NativeCallable::NativeCallable(std::string name, std::function<Value*(std::vector<Value*> arguments)> function) : Callable() {
    this->name = std::move(name);
    this->logic = std::move(function);
}

Value* NativeCallable::call(Interpreter* interpreter, std::vector<Value*> arguments) {
    return this->logic(arguments);
}

bool NativeCallable::equals(Value *other) {
    return other->has_type(ValueType::Func) && this->name == other->as<Callable>()->name;
}
