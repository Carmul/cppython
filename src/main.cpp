#include <iostream>
#include <string>
#include <fstream>

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "dotGenerator.h"

std::string readPythonFile(const std::string path);
void printDebugInfo(std::string script);
void printTokens(const std::string script);
void printAST(const std::string script);
void printDOT(const std::string script);
void interactiveMode();

int main() {
    auto script = readPythonFile("C:\\Users\\stalm\\source\\repos\\CCPython\\test.py");

	//printTokens(script);
	//return 0;

	Lexer lexer(script);
	Parser parser(lexer);
	ASTNodePtr tree = parser.parse();
	Interpreter interpreter(std::move(tree));

	printDOT(script);
	interpreter.interpret();

	interactiveMode();

    return 1;
}


// REPL - Read Eval Print Loop
void interactiveMode() {

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
					// If stmt is not AssignmentNode or PrintNode, treat it as expression
					if (!dynamic_cast<AssignmentNode*>(stmt.get()) && !dynamic_cast<PrintNode*>(stmt.get())) {
						if (result.isString())
							std::cout << "\"" + result.toString() + "\"" << std::endl;
						else std::cout << result.toString() << std::endl;
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

void printAST(const std::string script) {
	Lexer lexer = Lexer(script);
	Parser parser(lexer); // create parser
	ASTNodePtr tree = parser.parse(); // parse input to AST
	// print AST
	std::cout << "AST: " << tree->toString() << std::endl << std::endl;
}

void printDOT(const std::string script) {
	Lexer lexer = Lexer(script);
	Parser parser(lexer); // create parser
	ASTNodePtr tree = parser.parse(); // parse input to AST
	// generate and print DOT format
	std::string dot = DotGenerator().generate(std::move(tree));
	std::cout << "DOT format:\n" << dot << std::endl;
}

void printDebugInfo(std::string script) {

	printTokens(script);

	printAST(script);

	printDOT(script);
}
