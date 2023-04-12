cmake_minimum_required(VERSION 3.17)

project(BasicLinearAlgebra CXX)

## Build static library BLA

add_library(BLA
    include/bla/Matrix.hpp
    include/bla/Vector.hpp
    include/bla/Maths.h src/Maths.cpp)

## libfmt

find_library(FMT fmt PATHS ../../extlibs/fmt/build)
target_link_libraries(BLA ${FMT})
target_include_directories(BLA PRIVATE "../../extlibs/fmt/include")
target_compile_definitions(BLA PRIVATE ADD_FMT_FORMATTERS)


