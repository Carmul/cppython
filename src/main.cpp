#include <iostream>
#include <string>
#include <fstream>

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "dotGenerator.h"

std::string readPythonFile(const std::string path);
void printTokens(const std::string script);
void printDOT(const std::string script);
void replMode();

int main(int argc, char* argv[]) {

	if (argc == 1) {
		replMode();
		return 0;
	} 
	if (argc != 2) {
		std::cerr << "Usage: cppython <script.py>" << std::endl;
		return 1;
	}

    auto script = readPythonFile(argv[1]);
	
	Lexer lexer(script);
	Parser parser(lexer);
	ASTNodePtr tree = parser.parse();
	Interpreter interpreter(std::move(tree));

	try {
		Value result = interpreter.interpret();
	}
	catch (const std::exception& e) {
		std::cerr << "[-]	Error: " << e.what() << std::endl << std::endl;
		// print call stack
		if (interpreter.callStack.size() != 0) {
			std::cerr << "Call stack (most recent call last):" << std::endl;
			for (auto it = interpreter.callStack.rbegin(); it != interpreter.callStack.rend(); ++it) {
				std::cerr << "  in function '" << *it << "'" << std::endl;
			}
		}
	}

	replMode();

    return 1;
}


// REPL - Read Eval Print Loop
void replMode() {

	Interpreter interpreter(nullptr);
	std::cout << "\n\nCPPython Interpreter By Carmul (2025)\n(type 'exit' to quit)\n";

	while (true) {
		std::cout << ">>> ";
		std::string line;
		std::getline(std::cin, line);
		if (line == "exit" || line == "quit") break;
		if (line.empty()) continue;

		try {
			Lexer lexer(line);
			Parser parser(lexer);
			ASTNodePtr tree = parser.parse();
			interpreter.tree = std::move(tree); // saves the variables in the interpreter instance

			Value result = interpreter.interpret(); // interpret the AST

			// If the input is a single expression, print the result
			if (auto prog = dynamic_cast<BlockNode*>(interpreter.tree.get())) {
				if (prog->statements.size() == 1) {
					auto& stmt = prog->statements[0];
					// If stmt is not AssignmentNode, treat it as expression
					if (!dynamic_cast<AssignmentNode*>(stmt.get())) {
						if (result.isString())
							std::cout << "\"" + result.toString() + "\"" << std::endl;
						else 
							std::cout << result.toString() << std::endl;
					}
				}
			}
			
		}
		catch (const std::exception& e) {
			std::cerr << "[-]	Error: " << e.what() << std::endl;
		}
	}

	
}


std::string readPythonFile(const std::string path) {

    std::ifstream f(path);

    if (!f.is_open()) {
        std::cerr << "Error opening the file!";
        return "\0";
    }

    std::string line;
    std::string s = "";

    while (std::getline(f, line))
        s += line + "\n";

    return s;
}


void printTokens(const std::string script) {
	Lexer lexer = Lexer(script);
	// print tokens
	while (true) {
		Token t = lexer.getNextToken();
		std::cout << t.toString() << std::endl;
		if (t.type == TokenType::EOF_TOKEN)
			break;
	}
}


void printDOT(const std::string script) {
	Lexer lexer = Lexer(script);
	Parser parser(lexer); // create parser
	ASTNodePtr tree = parser.parse(); // parse input to AST
	// generate and print DOT format
	std::string dot = DotGenerator().generate(std::move(tree));
	std::cout << dot << std::endl;
}
