cmake_minimum_required(VERSION 3.17)

project(BasicLinearAlgebra CXX)

## Build static library BLA

set(LIB_NAME "BLA")
add_library(${LIB_NAME}
    include/bla/Matrix.hpp
    include/bla/Vector.hpp
    include/bla/Maths.h src/Maths.cpp)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20)
target_include_directories(${LIB_NAME} PRIVATE "include/bla")

## libfmt

find_library(FMT fmt PATHS ../../extlibs/fmt/build)
target_link_libraries(${LIB_NAME} ${FMT})
target_include_directories(${LIB_NAME} PRIVATE "../../extlibs/fmt/include")
target_compile_definitions(${LIB_NAME} PRIVATE ADD_FMT_FORMATTERS)


