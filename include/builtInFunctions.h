#pragma once

#include "ast.h"
#include "value.h"
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>

using BuiltinFunc = std::function<Value(const std::vector<Value>&)>;


Value printFunc(const std::vector<Value>& args);
Value printlnFunc(const std::vector<Value>& args);
Value maxFunc(const std::vector<Value>& args);
Value minFunc(const std::vector<Value>& args);

