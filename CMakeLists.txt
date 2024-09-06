cmake_minimum_required(VERSION 3.17)

project(LibraryShowcase CXX)


# GraphShowcase executable

set(EXE_NAME GraphShowcase)
add_executable(${EXE_NAME}
    src/${EXE_NAME}.cpp
    src/DrawableGraph.h src/DrawableGraph.cpp)

set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 20)
set_target_properties(${EXE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

## Graph lib
find_library(GRAPH graph PATHS libs/Graph/build)
target_link_libraries(${EXE_NAME} ${GRAPH})
target_include_directories(${EXE_NAME} PRIVATE "libs/Graph/include")

## {fmt}
find_library(FMT fmt PATHS extlibs/fmt/build)
target_link_libraries(${EXE_NAME} ${FMT})
target_include_directories(${EXE_NAME} PRIVATE "extlibs/fmt/include")
target_compile_definitions(${EXE_NAME} PRIVATE ADD_FMT_FORMATTERS)

## sfml
target_link_libraries(${EXE_NAME} sfml-graphics sfml-window sfml-system)



# GeometricAlgebraShowcase executable

set(EXE_NAME GeometricAlgebraShowcase)
add_executable(${EXE_NAME}
    src/${EXE_NAME}.cpp)

set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 20)
set_target_properties(${EXE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

## GeometricAlgebra lib
find_library(GA GeometricAlgebra PATHS libs/GeometricAlgebra/build)
target_link_libraries(${EXE_NAME} ${GA})
target_include_directories(${EXE_NAME} PRIVATE "libs/GeometricAlgebra/include")

## {fmt}
find_library(FMT fmt PATHS extlibs/fmt/build)
target_link_libraries(${EXE_NAME} ${FMT})
target_include_directories(${EXE_NAME} PRIVATE "extlibs/fmt/include")
target_compile_definitions(${EXE_NAME} PRIVATE ADD_FMT_FORMATTERS)

## sfml
target_link_libraries(${EXE_NAME} sfml-graphics sfml-window sfml-system)
