#include "radion/interpreter/stlib/stlib.hpp"

#include <iostream>
#include <functional>
#include <chrono>

#include "radion/interpreter/callable.hpp"
#include "radion/parser/nodes/literal.hpp"

#define NATIVE_FUNC(name, argname, content) top->put(name, new NativeCallable(name, [](std::vector<std::any> argname) { content }))

void print(std::vector<std::any> args) {
    for (int i = 0; i < args.size(); i++) {
        std::cout << value_to_string(args.at(i));
        if (i+1 < args.size()) std::cout << " ";
    }
}

void registerStandardLibrary(SymbolTable* top) {
    
    // Native functions
    top->put("print", new NativeCallable("print", [](std::vector<std::any> arguments)
                                                              {
        print(arguments);

        return NIL_VALUE; }));
    top->put("println", new NativeCallable("println", [](std::vector<std::any> arguments)
                                                               {
        print(arguments);
        std::cout << std::endl;

        return NIL_VALUE; }));

    NATIVE_FUNC("time", args, {
        long t = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        if (args.size() > 0 && args.at(0).type() == typeid(int))
            return (int)t % std::any_cast<int>(args.at(0));
        

        return (int)t;
    });
}
