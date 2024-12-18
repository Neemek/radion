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
    std::string name;

    std::string to_string() override;
};


class DefinedCallable : public Callable {
public:
    explicit DefinedCallable(DefinedCallable* callable);
    DefinedCallable(std::string name, std::vector<std::string> arguments, Node* logic);
    Value* call(Interpreter* interpreter, std::vector<Value*> arguments) override;

    bool equals(Value *other) override;
    Value * copy() override;
private:
    Node* logic;
    std::vector<std::string> arguments;
};

class NativeCallable : public Callable {
public:
    NativeCallable(std::string name, std::function<Value*(Interpreter *interpreter, std::vector<Value*> arguments)> function);
    Value* call(Interpreter* interpreter, std::vector<Value*> arguments) override;

    bool equals(Value *other) override;
    Value * copy() override;
private:
    std::function<Value*(Interpreter *interpreter, std::vector<Value*> arguments)> logic;
};

