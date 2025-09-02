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

int main() {
    
    auto script = readPythonFile("C:\\Users\\stalm\\source\\repos\\CCPython\\test");
    std::cout << "====================" << std::endl;
    std::cout << script << std::endl;
    std::cout << "====================" << std::endl << std::endl;


	Lexer lexer(script); // create lexer
	Parser parser(lexer); // create parser
	ASTNodePtr tree = parser.parse(); // parse input to AST
	Interpreter interpreter(std::move(tree)); // create interpreter
	auto result = interpreter.interpret(); // interpret the AST

	interpreter.printVariables();

	// interactive interpreter ONLY ONE LINE SCRIPTS
    std::string line;
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, line);
		if (line.empty())
			continue;
       
		Lexer lexer(line); // create lexer
		Parser parser(lexer); // create parser
		ASTNodePtr tree = parser.parse(); // parse input to AST
		Interpreter interpreter(std::move(tree)); // create interpreter
		auto result = interpreter.interpret(); // interpret the AST
		std::cout << std::to_string(result) << std::endl;

    }   

    return 0;
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
