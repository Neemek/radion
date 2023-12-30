#include <any>
#include <string>
#include <vector>
#include <functional>

#include "radion/interpreter/interpreter.hpp"
#include "radion/parser/nodes/functions.hpp"

class Callable {
public:
    virtual std::any call(Interpreter* interpreter, std::any arguments[]) = 0;
    virtual ~Callable();
};


class DefinedCallable : public Callable {
public:
    DefinedCallable(std::string arguments[], Node* logic);
    std::any call(Interpreter* interpreter, std::any arguments[]) override;
private:
    Node* logic;
    std::vector<std::string> arguments;
};

class NativeCallable : public Callable {
public:
    NativeCallable(std::function<std::any(std::any arguments[])> function);
    std::any call(Interpreter* interpreter, std::any arguments[]) override;
private:
    std::function<std::any(std::any arguments[])> logic;
};
