cmake_minimum_required(VERSION 3.28)

set(EXE_NAME FluidSim)
add_executable(${EXE_NAME}
    main.cpp
    FluidSim.h FluidSim.cpp)

set_target_properties(${EXE_NAME} PROPERTIES CXX_STANDARD 23 LINKER_LANGUAGE CXX)

foreach(LIB Interact)
    target_link_libraries(${EXE_NAME} ${LIB})
    target_include_directories(${EXE_NAME} PRIVATE "../../libs/${LIB}/include")
endforeach()

## fmt
target_link_libraries(${EXE_NAME} fmt)
target_include_directories(${EXE_NAME} PRIVATE "../../extlibs/fmt/include")
target_compile_definitions(${EXE_NAME} PUBLIC ADD_FMT_FORMATTERS)

## sfml
target_link_libraries(${EXE_NAME} SFML::Graphics SFML::Window SFML::System)
target_include_directories(${EXE_NAME} PRIVATE "../../extlibs/SFML/include")

target_compile_options(${EXE_NAME} PUBLIC ${COMPILE_OPTIONS}) # always
target_compile_options(${EXE_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>") # release
target_compile_options(${EXE_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>") # debug
target_link_options(${EXE_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_LINK_OPTIONS}>") # release
