cmake_minimum_required(VERSION 3.5.0)
project(server VERSION 0.1.0 LANGUAGES C CXX)

add_compile_definitions(DEBUG)

add_executable(server main.cpp server.cpp)

