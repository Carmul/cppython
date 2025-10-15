#include "builtInFunctions.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>


Value printFunc(const std::vector<Value>& args) {
    for (const auto& v : args) {
        std::cout << v.toString() << " ";
    }
    return Value(); // return nothing / null
}


Value printlnFunc(const std::vector<Value>& args) {
	printFunc(args);
    std::cout << std::endl; // extra newline
    return Value(); // return nothing / null
}


Value maxFunc(const std::vector<Value>& args) {
    if (args.empty()) {
        throw std::runtime_error("max() requires at least one argument");
    }
    double maxVal = args[0].asNumber();
    for (const auto& v : args) {
        double num = v.asNumber();
        if (num > maxVal) {
            maxVal = num;
        }
    }
    return Value(maxVal);
}


Value minFunc(const std::vector<Value>& args) {
    if (args.empty()) {
        throw std::runtime_error("min() requires at least one argument");
    }
    double minVal = args[0].asNumber();
    for (const auto& v : args) {
        double num = v.asNumber();
        if (num < minVal) {
            minVal = num;
        }
    }
    return Value(minVal);
}

Value nowFunc(const std::vector<Value>& args) {
    if (!args.empty()) {
        throw std::runtime_error("now() takes no arguments");
    }
    // Return current time as a string
	auto time = std::chrono::system_clock::now();

}