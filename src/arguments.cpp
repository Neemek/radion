#include "radion/arguments.hpp"
#include <iostream>

Arguments* parseArgs(int argc, char* argv[]) {
    Arguments* args = new Arguments;

    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];

        if (!strcmp(arg, "--dont-run")) {
            args->dontRun = true;
        } else if (!strcmp(arg, "--print-tokens")) {
            args->printTokens = true;
        } else {
            args->entryPath = arg;
        }
    }
    return args;
}