cmake_minimum_required(VERSION 2.8.12)

project(Zycore)

option(FORCE_SHARED_CRT "Forces shared linkage against the CRT" FALSE)

if (NOT CONFIGURED_ONCE)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
            "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(compiler_specific "-Werror")
        set(compiler_specific_cxx "-std=c++14")
    elseif (MSVC)
        set(compiler_specific "/WX /W4 /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS /GR-")
    endif ()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${compiler_specific} ${compiler_specific_cxx}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${compiler_specific}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif ()

# CMake always orders MSVC to build with a shared CRT. Hack CMake variables in order
# to generate with a statically linked CRT when we build as a static library.
if (MSVC AND NOT FORCE_SHARED_CRT)
    set(manipulated_vars
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_RELWITHDEBINFO)
    foreach (cur_var ${manipulated_vars})
        string(REPLACE "/MD" "/MT" ${cur_var} "${${cur_var}}")
    endforeach ()
endif ()

# Library
set(headers
    "include/binary_stream.hpp"
    "include/exceptions.hpp"
    "include/config.hpp"
    "include/operators.hpp"
    "include/optional.hpp"
    "include/property.hpp"
    "include/reflectable_object.hpp"
    "include/signal.hpp"
    "include/signal_object.hpp"
    "include/singleton.hpp"
    "include/tmp.hpp"
    "include/types.hpp"
    "include/type_traits.hpp"
    "include/utils.hpp")
set(sources
    "src/binary_stream.cpp"
    "src/property.cpp"
    "src/reflectable_object.cpp"
    "src/signal_object.cpp")

add_library("Zycore" ${headers} ${sources})
include_directories("include")

set(CONFIGURED_ONCE TRUE CACHE INTERNAL "CMake has configured at least once.")
