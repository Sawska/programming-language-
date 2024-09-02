cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(PROGRAMMING_LANGUAGE
    parser.cpp
    lexer.cpp
    AST.cpp
    token.h
    compiler.cpp
    main.cpp
)



option(ENABLE_DEBUG "Enable debugging" ON)

if (ENABLE_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()
