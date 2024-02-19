#include <iostream>
#include <fstream>
#include <sstream>
#include "radion/arguments.hpp"
#include "radion/lexer.hpp"
#include "radion/parser/parser.hpp"
#include "radion/interpreter/interpreter.hpp"
#include "radion/parser/nodes/block.hpp"

int main (int argc, char *argv[]) {
	Arguments* args = parseArgs(argc, argv);

#ifdef NDEBUG
	if (args->entryPath == nullptr) {
		// Help or REPL
		//vector<Token> tokens = Lexer::lex(src);

		string src;
		Parser p;
		Interpreter interpreter;

		bool replRunning = true;
		while (replRunning) {
			std::cout << "> ";
			std::getline(cin, src);

			vector<Token> tokens = Lexer::lex(src);
			p.reset(tokens, src);
			BlockNode* program = p.parse();

			if (p.hadError) continue;

			std::any value = interpreter.evaluate(program);
			string out;

			std::cout << value.type().name() << std::endl;
			if (value.type() == typeid(int)) {
				out = std::any_cast<int>(value);
			} else if (value.type() == typeid(string)) {
				out = std::any_cast<string>(value);
			}

			std::cout << out << std::endl;
		}
	} else 
#endif
	{
		// Run file
#ifndef NDEBUG
		ifstream t("/Users/neemek/Code/radion/examples/helloworld.rn");
#else
		ifstream t(args->entryPath);
#endif
		std::stringstream buffer;
		buffer << t.rdbuf();

		string src = buffer.str();
		vector<Token> tokens = Lexer::lex(src);

		if (args->printTokens) {
			std::cout << "-- tokens --" << std::endl;
			print_tokens(src, tokens);
			std::cout << "-- program output --" << std::endl;
		}

		Parser p(tokens, src);
		BlockNode* program = p.parse();

		if (p.hadError) {
			std::cout << "Program had errors, not evaluating" << std::endl;
			return 1;
		}

		if (args->dontRun) {
			std::cout << "not running" << std::endl;
			return 0;
		}

		Interpreter interpreter;
		try {
			interpreter.evaluate(program);
		} catch (RuntimeException &e) { // catch error messages
			e.print(src);
			return 1;
		}
	}
	
	return 0;
}

