#include <iostream>
#include <string>
#include <fstream>

#include "lexer.h"

std::string readPythonFile(const std::string path);

int main() {

    auto script = readPythonFile("C:\\Users\\stalm\\source\\repos\\CCPython\\test");
    std::cout << "====================" << std::endl;
    std::cout << script << std::endl;
    std::cout << "====================" << std::endl << std::endl;

    Lexer lexer = Lexer(script);

    while (true) {
        Token t = lexer.getNextToken();
        std::cout << t.toString() << std::endl;
        if (t.type == TokenType::EOF_TOKEN)
            break;
    }
    std::cout << std::endl;

    // interactive interpreter
    std::string line;
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, line);
        Lexer lexer = Lexer(line);

        while (true) {
            Token t = lexer.getNextToken();
            std::cout << t.toString() << std::endl;
            if (t.type == TokenType::EOF_TOKEN)
                break;
        }
        std::cout << std::endl;
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