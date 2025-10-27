cmake_minimum_required(VERSION 3.17)

set(EXE_NAME BlueVoronoi)
add_executable(${EXE_NAME}
    main.cpp)

set_target_properties(${EXE_NAME} PROPERTIES CXX_STANDARD 20 LINKER_LANGUAGE CXX)

## fmt
target_link_libraries(${EXE_NAME} fmt)
target_include_directories(${EXE_NAME} PRIVATE "../../extlibs/fmt/include")
target_compile_definitions(${EXE_NAME} PUBLIC ADD_FMT_FORMATTERS)

## sfml
target_link_libraries(${EXE_NAME} SFML::Graphics SFML::Window SFML::System)
target_include_directories(${EXE_NAME} PRIVATE "../../extlibs/SFML/include")

## voronoi
target_include_directories(${EXE_NAME} PRIVATE "../../extlibs/voronoi/src")

target_compile_options(${EXE_NAME} PUBLIC ${COMPILE_OPTIONS}) # always
target_compile_options(${EXE_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>") # release
target_compile_options(${EXE_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>") # debug
target_link_options(${EXE_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_LINK_OPTIONS}>") # release
