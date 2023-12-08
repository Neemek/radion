#include <iostream>
#include <fstream>
#include <sstream>
#include "radion/lexer.hpp"
#include "radion/parser/parser.hpp"

int main (int argc, char *argv[]) {
	if (argc <= 1) {
		// Help or REPL
	} else if (argc == 2) {
		// Run file
		ifstream t(argv[1]);
		std::stringstream buffer;
		buffer << t.rdbuf();
		
		string src = buffer.str();

		vector<Token> tokens = Lexer::lex(src);

		Parser p(tokens, src);

		Node program = p.parse();

		
	}
	
	return 0;
}

