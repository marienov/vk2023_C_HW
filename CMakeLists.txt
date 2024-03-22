cmake_minimum_required(VERSION 3.0.0)
project(VK2023_C_HW VERSION 0.1.0)

add_subdirectory(lib/utils)

add_executable(VK2023_C_HW src/main.cpp)
target_link_libraries(VK2023_C_HW utils)
