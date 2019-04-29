cmake_minimum_required(VERSION 3.10.2)

project (test_cmake_project)

add_subdirectory(lib)
include_directories(lib)

add_executable(test_a TFD.c)
target_link_libraries(test_a liba)
