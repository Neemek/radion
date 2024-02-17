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

std::any DefinedCallable::call(Interpreter* interpreter, std::vector<std::any> arguments) {
    interpreter->table_descend();
    for (int i = 0; i < arguments.size(); i++)
    {
        interpreter->symbols->put(this->arguments.at(i), arguments.at(i));
    }
    

    interpreter->nofree = true;
    std::any ret = interpreter->evaluate(this->logic);
    interpreter->nofree = false;

    interpreter->table_ascend();
    return ret;
}


NativeCallable::NativeCallable(const char* name, std::function<std::any(std::vector<std::any> arguments)> function) : Callable() {
    this->name = name;
    this->logic = std::move(function);
}

std::any NativeCallable::call(Interpreter* interpreter, std::vector<std::any> arguments) {
    return this->logic(arguments);
}

Callable* get_callable(std::any value) {
    Callable* callable;
    try {
        callable = std::any_cast<NativeCallable*>(value);
    } catch (const std::bad_any_cast& _e) {
        try {
            callable = std::any_cast<DefinedCallable*>(value);
        } catch (const std::bad_any_cast& e) {
            throw new RuntimeException("value is not a callable");
        }
    }

    return callable;
}


