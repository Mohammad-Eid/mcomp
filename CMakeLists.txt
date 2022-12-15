cmake_minimum_required(VERSION 3.15)
project(mcomp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(mcomp
        ast.h
        lex.yy.c
        main.cpp
        main.h
        miny.tab.cpp
        miny.tab.hpp)
