cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)
project(Imaging VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)

add_subdirectory(src)



enable_testing()
add_subdirectory(tests) 
include(GoogleTest)