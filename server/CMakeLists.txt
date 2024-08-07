cmake_minimum_required(VERSION 3.5.0)
project(server VERSION 0.1.0 LANGUAGES CXX)

add_compile_definitions(USE_PARAMS)
add_compile_definitions(QUEUE_NODES=100)

find_package(Boost 1.76 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(server main.cpp server.cpp)
else()
    message(ERROR "Boost not found")
endif()
