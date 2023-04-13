cmake_minimum_required(VERSION 3.17)

project(LibraryShowCase CXX)

## Build test executable

add_executable(test tests/main.cpp)
set_property(TARGET test PROPERTY CXX_STANDARD 20)

## bla
find_library(BLA BLA PATHS libs/bla/build)
target_link_libraries(test ${BLA})
target_include_directories(test PRIVATE "libs/bla/include")
target_compile_definitions(test PRIVATE ADD_FMT_FORMATTERS)

## graph
find_library(GRAPH graph PATHS libs/graph/build)
target_link_libraries(test ${GRAPH})
target_include_directories(test PRIVATE "libs/graph/include")
target_compile_definitions(test PRIVATE ADD_FMT_FORMATTERS)

## libfmt
find_library(FMT fmt PATHS extlibs/fmt/build)
target_link_libraries(test ${FMT})
target_include_directories(test PRIVATE "extlibs/fmt/include")

