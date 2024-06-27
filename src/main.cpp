#include <iostream>
#include <fstream>
#include <sstream>
#include "radion/arguments.hpp"
#include "radion/lexer.hpp"
#include "radion/parser/parser.hpp"
#include "radion/interpreter/interpreter.hpp"
#include "radion/interpreter/callable.hpp"
#include "radion/interpreter/values/constant.hpp"
#include "radion/parser/nodes/block.hpp"

int main (int argc, char *argv[]) {
	Arguments* args = parseArgs(argc, argv);

	if (args->entryPath == nullptr) {
        // Help or REPL
		//vector<Token> tokens = Lexer::lex(src);

		string src;
		Parser p;
        BlockNode* program;
		Interpreter interpreter;
        Value *returned;

        bool replRunning = true;
        interpreter.symbols->put("exit",new NativeCallable("exit", [&replRunning](Interpreter *interpreter, const std::vector<Value*>& args) {
            // doesnt depend on arguments; Alex B. https://stackoverflow.com/questions/1486904/how-do-i-best-silence-a-warning-about-unused-variables
            (void)interpreter;
            (void)args;

            replRunning = false;
            return NIL_VALUE;
        }));

		while (replRunning) {
			std::cout << "> ";
			std::getline(cin, src);

			vector<Token> tokens = Lexer::lex(src);
            // if no tokens are parsed (only END)
            if (tokens.size() == 1) continue;
			p.reset(tokens, src);
			program = p.parse();

			if (p.hadError) continue;

            try {
                for (Node *statement : program->statements) {
                    returned = interpreter.evaluate(statement);

                    std::cout << returned->to_string() << std::endl;
                }
            } catch (RuntimeException &e) {
                e.print(src);
            }
		}
	} else {
		// Run file
		ifstream t(args->entryPath);
		std::stringstream buffer;
		buffer << t.rdbuf();

		string src = buffer.str();
		vector<Token> tokens = Lexer::lex(src);

		if (args->printTokens) {
			std::cout << "-- tokens --" << std::endl;
			print_tokens(src, tokens);
			std::cout << "-- end tokens --" << std::endl;
		}

		Parser p(tokens, src);
		BlockNode* program = p.parse();

		if (p.hadError) {
			std::cout << "Program had errors, not evaluating" << std::endl;
			return 1;
		}

        if (args->printAST) {
            std::cout << "-- ast --"; // printing the ast starts with a new line
            printAST(program);
            std::cout << std::endl << "-- end ast --" << std::endl;
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

