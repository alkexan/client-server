# Main CMakeList.txt
cmake_minimum_required(VERSION 3.5.0)

# set language and the project name
project (client-server CXX)

# CXX standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include subprojects
add_compile_definitions(DEBUG)

add_subdirectory (client)
add_subdirectory (server)
