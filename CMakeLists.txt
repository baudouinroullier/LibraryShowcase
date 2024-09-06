cmake_minimum_required(VERSION 3.17)

project(LibraryShowcase CXX)

## Build test executable

add_executable(GraphShowcase
    src/main.cpp
    src/DrawableGraph.h src/DrawableGraph.cpp)

set_property(TARGET GraphShowcase PROPERTY CXX_STANDARD 20)
set_target_properties(GraphShowcase PROPERTIES LINKER_LANGUAGE CXX)

# graph
find_library(GRAPH graph PATHS libs/Graph/build)
target_link_libraries(GraphShowcase ${GRAPH})
target_include_directories(GraphShowcase PRIVATE "libs/Graph/include")
target_compile_definitions(GraphShowcase PRIVATE ADD_FMT_FORMATTERS)

## libfmt
find_library(FMT fmt PATHS extlibs/fmt/build)
target_link_libraries(GraphShowcase ${FMT})
target_include_directories(GraphShowcase PRIVATE "extlibs/fmt/include")

## sfml
target_link_libraries(GraphShowcase sfml-graphics sfml-window sfml-system)

# ## bla
# find_library(BLA BLA PATHS libs/bla/build)
# target_link_libraries(GraphShowcase ${BLA})
# target_include_directories(GraphShowcase PRIVATE "libs/bla/include")
# target_compile_definitions(GraphShowcase PRIVATE ADD_FMT_FORMATTERS)

