#include <any>
#include <string>
#include <vector>
#include <functional>

#include "radion/interpreter/interpreter.hpp"
#include "radion/interpreter/value.hpp"
#include "radion/parser/nodes/functions.hpp"

class Callable : public Value {
public:
    Callable() : Value(ValueType::Func) {};
    virtual std::any call(Interpreter* interpreter, std::vector<std::any> arguments)=0;
    const char* name;

    std::string to_string() override;
};


class DefinedCallable : public Callable {
public:
    DefinedCallable(DefinedCallable* callable);
    DefinedCallable(const char* name, std::vector<std::string> arguments, Node* logic);
    std::any call(Interpreter* interpreter, std::vector<std::any> arguments) override;
private:
    Node* logic;
    std::vector<std::string> arguments;
};

class NativeCallable : public Callable {
public:
    NativeCallable(const char* name, std::function<std::any(std::vector<std::any> arguments)> function);
    std::any call(Interpreter* interpreter, std::vector<std::any> arguments) override;
private:
    std::function<std::any(std::vector<std::any> arguments)> logic;
};

Callable* get_callable(std::any value);
