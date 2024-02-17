#include <string>
#include <vector>
#include <functional>

#include "radion/interpreter/interpreter.hpp"
#include "radion/interpreter/value.hpp"
#include "radion/parser/nodes/functions.hpp"

class Callable : public Value {
public:
    Callable() : Value(ValueType::Func) {};
    virtual Value* call(Interpreter* interpreter, std::vector<Value*> arguments)=0;
    const char* name;

    std::string to_string() override;
};


class DefinedCallable : public Callable {
public:
    DefinedCallable(DefinedCallable* callable);
    DefinedCallable(const char* name, std::vector<std::string> arguments, Node* logic);
    Value* call(Interpreter* interpreter, std::vector<Value*> arguments) override;

    bool equals(Value *other) override;
private:
    Node* logic;
    std::vector<std::string> arguments;
};

class NativeCallable : public Callable {
public:
    NativeCallable(const char* name, std::function<Value*(std::vector<Value*> arguments)> function);
    Value* call(Interpreter* interpreter, std::vector<Value*> arguments) override;

    bool equals(Value *other) override;
private:
    std::function<Value*(std::vector<Value*> arguments)> logic;
};

