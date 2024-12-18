#include "radion/arguments.hpp"
#include <cstring>

Arguments* parseArgs(int argc, char* argv[]) {
    auto* args = new Arguments;

    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];

        if (!strcmp(arg, "--dont-run") || !strcmp(arg, "--dry")) {
            args->dontRun = true;
        } else if (!strcmp(arg, "--print-tokens")) {
            args->printTokens = true;
        } else if (!strcmp(arg, "--print-ast")) {
            args->printAST = true;
        } else {
            args->entryPath = arg;
        }
    }
    return args;
}
