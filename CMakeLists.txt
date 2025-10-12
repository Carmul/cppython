cmake_minimum_required(VERSION 3.15)
project(CPPython LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Token.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/interpreter.cpp
    src/dotGenerator.cpp
    src/builtInFunctions.cpp
    src/value.cpp
    )

# Header files
set(HEADERS
    include/Token.h
    include/lexer.h
    include/parser.h
    include/ast.h 
    include/interpreter.h
    include/dotGenerator.h
    include/builtInFunctions.h
    include/value.h
)

add_executable(CPPython ${SOURCES} ${HEADERS})
target_include_directories(CPPython PRIVATE include)