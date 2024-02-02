#include "radion/interpreter/callable.hpp"
#include <algorithm>
#include <iterator>

Callable::~Callable() {
}


DefinedCallable::DefinedCallable(const char* name, std::string arguments[], Node* logic) {
    this->name = name;
    std::copy(std::begin(*arguments), std::end(*arguments), std::begin(this->arguments));
    this->logic = logic;
}

std::any DefinedCallable::call(Interpreter* interpreter, std::any arguments[]) {

    return interpreter->evaluate(this->logic);
}


NativeCallable::NativeCallable(const char* name, std::function<std::any(std::any arguments[])> function) {
    this->name = name;
    this->logic = function;
}

std::any NativeCallable::call(Interpreter* interpreter, std::any arguments[]) {
    return this->logic(arguments);
}
