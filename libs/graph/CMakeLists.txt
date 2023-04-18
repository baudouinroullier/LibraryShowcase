cmake_minimum_required(VERSION 3.17)

project(GraphLib CXX)

## Build static library

set(LIB_NAME "graph")
add_library(${LIB_NAME}
    include/graph/GraphItf.h
    include/graph/GraphWithPointer.h src/GraphWithPointer.cpp
)
target_include_directories(${LIB_NAME} PRIVATE "include/graph")

## libfmt

find_library(FMT fmt PATHS ../../extlibs/fmt/build)
target_link_libraries(${LIB_NAME} ${FMT})
target_include_directories(${LIB_NAME} PRIVATE "../../extlibs/fmt/include")
target_compile_definitions(${LIB_NAME} PRIVATE ADD_FMT_FORMATTERS)


