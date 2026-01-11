cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)
project(Imaging VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


add_subdirectory(src)

qt_standard_project_setup()

enable_testing()
add_subdirectory(tests) 
include(GoogleTest)
