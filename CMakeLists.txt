cmake_minimum_required(VERSION 3.17)

project(BasicLinearAlgebra)

## Build static library BLA

add_library(BLA
    src/Matrix.h src/Matrix.cpp
    src/Vector.h src/Vector.cpp)

## Build test executable

add_executable(test tests/main.cpp)
set_property(TARGET test PROPERTY CXX_STANDARD 20)

target_link_libraries(test BLA)

## libfmt

find_library(FMT fmt PATHS extlibs/fmt/build)
target_link_libraries(BLA ${FMT})
target_link_libraries(test ${FMT})
target_include_directories(BLA PRIVATE "extlibs/fmt/include")
target_include_directories(test PRIVATE "extlibs/fmt/include")
target_compile_definitions(BLA PRIVATE ADD_FMT_FORMATTERS)
target_compile_definitions(test PRIVATE ADD_FMT_FORMATTERS)

