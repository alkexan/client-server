cmake_minimum_required(VERSION 3.5.0)
project(server VERSION 0.1.0 LANGUAGES CXX)

add_compile_definitions(USE_PARAMS)
add_executable(server main.cpp server.cpp)
