#include "value.h"
#include <cmath>

// Constructors
Value::Value() : data(0.0) {}
Value::Value(double d) : data(d) {}
Value::Value(bool b) : data(b) {}
Value::Value(const std::string& s) : data(s) {}
Value::Value(const char* s) : data(std::string(s)) {}

// Type checking
bool Value::isNumber() const { return std::holds_alternative<double>(data); }
bool Value::isBool() const { return std::holds_alternative<bool>(data); }
bool Value::isString() const { return std::holds_alternative<std::string>(data); }

// Type accessors with error checking
double Value::asNumber() const {
    if (auto* val = std::get_if<double>(&data)) return *val;
    if (auto* val = std::get_if<bool>(&data)) return *val ? 1.0 : 0.0;
    throw std::runtime_error("Value is not a number");
}

bool Value::asBool() const {
    if (auto* val = std::get_if<bool>(&data)) return *val;
    throw std::runtime_error("Value is not a boolean");
}

std::string Value::asString() const {
    if (auto* val = std::get_if<std::string>(&data)) return *val;
    throw std::runtime_error("Value is not a string");
}

// Convert to string for printing
std::string Value::toString() const {
    if (isNumber()) {
        if (asNumber() == std::trunc(asNumber())) // integer
            return std::to_string(static_cast<int>(asNumber()));
		else
            return std::to_string(asNumber());
    }
    else if (isBool()) return asBool() ? "True" : "False";
    else if (isString()) return asString();
    return "undefined";
}

// Type name for error messages
std::string Value::typeName() const {
    if (isNumber()) return "number";
    if (isBool()) return "bool";
    if (isString()) return "string";
    return "unknown";
}

// Truthiness (for conditions)
bool Value::isTruthy() const {
    if (isBool()) return asBool();
    if (isNumber()) return asNumber() != 0.0;
    if (isString()) return !asString().empty();
    return false;
}
