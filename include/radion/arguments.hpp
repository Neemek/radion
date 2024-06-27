#include <map>

struct Arguments {
    char* entryPath{};

    bool printTokens = false;
    bool dontRun = false;
    bool printAST = false;
};

Arguments* parseArgs(int argc, char* argv[]);
