cmake_minimum_required(VERSION 3.0.0)
project(hw1 VERSION 0.1.0)

add_subdirectory(lib/utils)

add_executable(hw1 src/main.cpp)
target_link_libraries(hw1 utils)