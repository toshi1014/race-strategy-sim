cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(race-strategy-sim)

set(SRC_DIR src)
set(EXE_FILENAME a.out)

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

include(FetchContent)
FetchContent_Declare(
    json
    URL
    https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
)
FetchContent_MakeAvailable(json)

target_include_directories(
    mylib
    PRIVATE
    ${CMAKE_BINARY_DIR}/_deps/json-src/single_include
)

add_executable(
    ${EXE_FILENAME}
    ${SRC_DIR}/main.cpp
)

target_link_libraries(
    ${EXE_FILENAME}
    mylib
    nlohmann_json::nlohmann_json
)