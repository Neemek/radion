#include "radion/interpreter/stlib/stlib.hpp"

#include <iostream>
#include <functional>
#include <utility>
#include <vector>
#include <chrono>

#include "radion/interpreter/callable.hpp"
#include "radion/interpreter/values/constant.hpp"
#include "radion/interpreter/values/number.hpp"
#include "radion/interpreter/values/list.hpp"



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
        print(std::move(arguments));

        return NIL_VALUE; }));
    top->put("println", new NativeCallable("println", [](std::vector<Value*> arguments)
                                                               {
        print(std::move(arguments));
        std::cout << std::endl;

        return NIL_VALUE; }));


    top->put("time", new NativeCallable("time", [](std::vector<Value*> args) {
        long t = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        if (!args.empty() && args.at(0)->get_type() == ValueType::Int)
            return new IntValue((int)t % args.at(0)->as<IntValue>()->number);
        

        return new IntValue((int)t);
    }));

    top->put("zip", new NativeCallable("zip", [](const std::vector<Value*>& args) {
        auto *zipped = new ListValue;

        int max_size = 0;
        for (Value *arg : args) {
            auto *listArg = arg->expect_type(ValueType::List)->as<ListValue>();
            if (listArg->elements.size() > max_size) max_size = (int)listArg->elements.size();
        }
        while (zipped->elements.size() < max_size) zipped->elements.push_back(new ListValue);

        for (Value *arg : args) {
            auto *l = arg->as<ListValue>();
            std::vector<Value*> elements = l->elements;

            for (int i = 0; i < zipped->elements.size(); ++i) {
                auto layer = zipped->elements.at(i)->as<ListValue>();

                if (elements.size() <= i) {
                    layer->elements.push_back(NIL_VALUE);
                    continue;
                }

                auto value = elements.at(i);

                layer->elements.push_back(value);
            }
        }

        return zipped;
    }));
}
