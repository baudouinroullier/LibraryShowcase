cmake_minimum_required(VERSION 3.17)

project(BasicLinearAlgebra)

add_library(BLA
    src/bla.h src/bla.cpp
    src/Matrix.h src/Matrix.cpp)


add_executable(test tests/main.cpp)
target_link_libraries(test BLA)

set_property(TARGET test PROPERTY CXX_STANDARD 20)
