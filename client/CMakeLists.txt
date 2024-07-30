cmake_minimum_required(VERSION 3.5.0)
project(client VERSION 0.1.0 LANGUAGES CXX)

add_compile_definitions(USE_PARAMS)
add_executable(client main.cpp client.cpp)
