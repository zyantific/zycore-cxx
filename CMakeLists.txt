cmake_minimum_required(VERSION 3.0)

project(Zycore)

option(FORCE_SHARED_CRT "Forces shared linkage against the CRT" FALSE)
option(HEADER_ONLY "Configure for header-only library usage" FALSE)

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

if (HEADER_ONLY)
    add_definitions("-DZYCORE_HEADER_ONLY=1")
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
    "include/BinaryStream.hpp"
    "include/Exceptions.hpp"
    "include/Config.hpp"
    "include/Operators.hpp"
    "include/Optional.hpp"
    "include/Property.hpp"
    "include/ReflectableObject.hpp"
    "include/Signal.hpp"
    "include/SignalObject.hpp"
    "include/Singleton.hpp"
    "include/Mpl.hpp"
    "include/Result.hpp"
    "include/Types.hpp"
    "include/TypeTraits.hpp"
    "include/Utils.hpp")
set(sources
    "src/BinaryStream.cpp"
    "src/Property.cpp"
    "src/ReflectableObject.cpp"
    "src/SignalObject.cpp")

if (HEADER_ONLY)
    add_library("Zycore" INTERFACE)
    target_include_directories("Zycore" INTERFACE "include/")
else ()
    add_library("Zycore" ${headers} ${sources})
    include_directories("include/")
endif ()

set(CONFIGURED_ONCE TRUE CACHE INTERNAL "CMake has configured at least once.")
