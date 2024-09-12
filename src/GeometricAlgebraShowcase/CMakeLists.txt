cmake_minimum_required(VERSION 3.17)

set(EXE_NAME GeometricAlgebraShowcase)
add_executable(${EXE_NAME}
    main.cpp)

set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 20)
set_target_properties(${EXE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

## GeometricAlgebra lib
find_library(GA GeometricAlgebra PATHS ../../libs/GeometricAlgebra/build)
target_link_libraries(${EXE_NAME} ${GA})
target_include_directories(${EXE_NAME} PRIVATE "../../libs/GeometricAlgebra/include")

## {fmt}
find_library(FMT fmt PATHS ../../extlibs/fmt/build)
target_link_libraries(${EXE_NAME} ${FMT})
target_include_directories(${EXE_NAME} PRIVATE "../../extlibs/fmt/include")
target_compile_definitions(${EXE_NAME} PRIVATE ADD_FMT_FORMATTERS)

## sfml
target_link_libraries(${EXE_NAME} sfml-graphics sfml-window sfml-system)

## Act lib
find_library(INTERACT Interact PATHS ../../libs/Interact/build)
target_link_libraries(${EXE_NAME} ${INTERACT})
target_include_directories(${EXE_NAME} PRIVATE "../../libs/Interact/include")
