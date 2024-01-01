#include <iostream>
#include <fstream>
#include <sstream>
#include "radion/lexer.hpp"
#include "radion/parser/parser.hpp"
#include "radion/interpreter/interpreter.hpp"
#include "radion/parser/nodes/block.hpp"

int main (int argc, char *argv[]) {
	if (argc <= 1) {
		// Help or REPL
		//vector<Token> tokens = Lexer::lex(src);

		string src;
		Parser p;
		Interpreter interpreter;

		bool replRunning = true;
		while (replRunning) {
			std::cout << "> ";
			std::cin >> src;

			vector<Token> tokens = Lexer::lex(src);
			p.reset(tokens, src);
			BlockNode program = p.parse();

			if (p.hadError) continue;

			interpreter.evaluate(&program);
			src.clear();
		}
	} else if (argc == 2) {
		// Run file
		ifstream t(argv[1]);
		std::stringstream buffer;
		buffer << t.rdbuf();

		string src = buffer.str();
		vector<Token> tokens = Lexer::lex(src);

		Parser p(tokens, src);
		BlockNode program = p.parse();

		if (p.hadError) {
			std::cout << "Program had errors, not evaluating" << std::endl;
			return 1;
		}

		Interpreter interpreter;
		interpreter.evaluate(&program);
	}
	
	return 0;
}

