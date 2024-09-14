cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(PROGRAMMING_LANGUAGE
    main.cpp
    parser.cpp
    lexer.cpp
    AST.cpp
    token.h
    compiler.cpp
    SymbolTable.cpp   
    StringNode.cpp
    NumberNode.cpp
    EmptyNode.cpp
    VariableNode.cpp
    UnaryOperationNode.cpp
    BinaryOperationNode.cpp
    BlockNode.cpp   
    ReturnNode.cpp
    ifNode.cpp
    WhileNode.cpp
    ContinueNode.cpp
    BreakNode.cpp
)



option(ENABLE_DEBUG "Enable debugging" ON)

if (ENABLE_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()
