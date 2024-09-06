cmake_minimum_required(VERSION 3.17)

set(EXE_NAME GraphShowcase)
add_executable(${EXE_NAME}
    main.cpp
    DrawableGraph.h DrawableGraph.cpp)

set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 20)
set_target_properties(${EXE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

## Graph lib
find_library(GRAPH graph PATHS ../../libs/Graph/build)
target_link_libraries(${EXE_NAME} ${GRAPH})
target_include_directories(${EXE_NAME} PRIVATE "../../libs/Graph/include")

## {fmt}
find_library(FMT fmt PATHS ../../extlibs/fmt/build)
target_link_libraries(${EXE_NAME} ${FMT})
target_include_directories(${EXE_NAME} PRIVATE "../../extlibs/fmt/include")
target_compile_definitions(${EXE_NAME} PRIVATE ADD_FMT_FORMATTERS)

## sfml
target_link_libraries(${EXE_NAME} sfml-graphics sfml-window sfml-system)
