cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(MyProj)

set(SRC_DIR src)

add_compile_options(
    -std=c++17
    -Wall
    --pedantic-error
)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

FILE(GLOB CPP_FILE ${SRC_DIR}/*.cpp)
add_library(
    mylib
    STATIC
    ${CPP_FILE}
)

add_executable(a.out ${SRC_DIR}/main.cpp)
target_link_libraries(a.out mylib)