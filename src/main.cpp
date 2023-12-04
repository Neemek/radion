#include <iostream>
#include <fstream>
#include "radion/lexer.hpp"

int main (int argc, char *argv[]) {
	if (argc <= 1) {
		// Help or REPL
	} else if (argc == 2) {
		// Run file
		ifstream f(argv[1]);
		char src[1 << 12];
		f >> src;

		Lexer lexer;
		vector<Token> tokens = lexer.lex(src);

		// test
	}
	
	return 0;
}

