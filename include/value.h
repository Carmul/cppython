#pragma once

#include <variant>
#include <string>
#include <stdexcept>

// TODO: add None type 
class Value {
public:
    using ValueType = std::variant<double, bool, std::string>;

private:
    ValueType data;

public:
    // Constructors
    Value();
    Value(double d);
    Value(bool b);
    Value(const std::string& s);
    Value(const char* s);

    // Type checking
    bool isNumber() const;
    bool isBool() const;
    bool isString() const;

    // Type accessors with error checking
    double asNumber() const;
    bool asBool() const;
    std::string asString() const;

    // Convert to string for printing
    std::string toString() const;

    // Type name for error messages
    std::string typeName() const;

    // Truthiness (for conditions)
    bool isTruthy() const;
};
