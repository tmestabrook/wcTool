cmake_minimum_required(VERSION 3.24)
project(wcTool)

set(CMAKE_CXX_STANDARD 17)

add_executable(wcTool main.cpp wcObj.h wcObj.cpp)