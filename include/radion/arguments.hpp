#include <map>

struct Arguments {
    char* entryPath;

    bool printTokens = false;
    bool dontRun = false;
};

Arguments* parseArgs(int argc, char* argv[]);
