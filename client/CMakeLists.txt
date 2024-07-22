cmake_minimum_required(VERSION 3.5.0)
project(client VERSION 0.1.0 LANGUAGES C CXX)

add_compile_definitions(DEBUG)

add_executable(client main.cpp client.cpp)

