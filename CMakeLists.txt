cmake_minimum_required(VERSION 3.17)

project(LibraryShowcase CXX)

if(NOT CMAKE_BUILD_TYPE)
    message("Forcing Release build")
    set(CMAKE_BUILD_TYPE Release)
endif()

set(COMPILE_OPTIONS
    "-Wextra" "-Wall"
    "-Wshadow" "-Wpointer-arith"
    "-Wno-deprecated-declarations" "-Wno-format-truncation"
    "-fno-omit-frame-pointer" #"-fsanitize=address,undefined"
    "-U_FORTIFY_SOURCE")
set(DEBUG_COMPILE_OPTIONS "-g")
set(RELEASE_COMPILE_OPTIONS "-O3" "-ffast-math" "-march=native")
set(RELEASE_LINK_OPTIONS "-flto")

foreach(PROJECT GraphShowcase GeometricAlgebraShowcase BlueVoronoi) # SFMLImGuiShowcase
    add_subdirectory(src/${PROJECT} ${PROJECT})
endforeach()

## libs
foreach(LIB Graph Interact GeometricAlgebra)
    add_subdirectory(libs/${LIB} ${LIB})
endforeach()

## extlibs
foreach(LIB fmt)
    add_subdirectory(extlibs/${LIB} ${LIB})
endforeach()
