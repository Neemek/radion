#include "radion/interpreter/stlib/stlib.hpp"

#include <iostream>
#include <functional>
#include <vector>
#include <chrono>

#include "radion/interpreter/callable.hpp"
#include "radion/interpreter/values/constant.hpp"
#include "radion/interpreter/values/number.hpp"
#include "radion/interpreter/values/list.hpp"

#define NATIVE_FUNC(name, argname, content) top->put(name, new NativeCallable(name, [](std::vector<Value*> argname) { content }))

void print(std::vector<Value*> args) {
    for (int i = 0; i < args.size(); i++) {
        std::cout << args.at(i)->to_string();
        if (i+1 < args.size()) std::cout << " ";
    }
}

void registerStandardLibrary(SymbolTable* top) {
    
    // Native functions
    top->put("print", new NativeCallable("print", [](std::vector<Value*> arguments)
                                                              {
        print(arguments);

        return NIL_VALUE; }));
    top->put("println", new NativeCallable("println", [](std::vector<Value*> arguments)
                                                               {
        print(arguments);
        std::cout << std::endl;

        return NIL_VALUE; }));

    NATIVE_FUNC("time", args, {
        long t = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        if (!args.empty() && args.at(0)->get_type() == ValueType::Int)
            return new IntValue((int)t % args.at(0)->as<IntValue>()->number);
        

        return new IntValue((int)t);
    });

    NATIVE_FUNC("zip", args, {
        auto *zipped = new ListValue;

        for (Value *arg : args) {
            auto *l = arg->expect_type(ValueType::List)->as<ListValue>();
            std::vector<Value*> elements = l->elements;

            for (int i = 0; i < elements.size(); ++i) {
                auto value = elements.at(i);

                if (zipped->elements.size() >= i) {
                    elements.push_back(new ListValue());
                }

                auto layer = elements.at(i);

                layer->as<ListValue>()->elements.push_back(value);
            }
        }

        return zipped;
    });
}
