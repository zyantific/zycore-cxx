cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0054 NEW)
project(Zycore)

option(ZYCORE_FORCE_SHARED_CRT "Forces shared linkage against the CRT" FALSE)
option(ZYCORE_HEADER_ONLY "Configure for header-only library usage" FALSE)

option(ZYCORE_DEV "Enable ZyCore development mode (warnings -> errors, ...)" FALSE)
mark_as_advanced(ZYCORE_DEV)

if (ZYCORE_HEADER_ONLY)
    add_definitions("-DZYCORE_HEADER_ONLY=1")
endif ()

# CMake always orders MSVC to build with a shared CRT. Hack CMake variables in order
# to generate with a statically linked CRT when we build as a static library.
if (MSVC AND NOT ZYCORE_FORCE_SHARED_CRT)
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
    "include/zycore/BinaryStream.hpp"
    "include/zycore/Exceptions.hpp"
    "include/zycore/Config.hpp"
    "include/zycore/Operators.hpp"
    "include/zycore/Optional.hpp"
    "include/zycore/Property.hpp"
    "include/zycore/ReflectableObject.hpp"
    "include/zycore/Signal.hpp"
    "include/zycore/SignalObject.hpp"
    "include/zycore/Singleton.hpp"
    "include/zycore/Mpl.hpp"
    "include/zycore/Result.hpp"
    "include/zycore/Types.hpp"
    "include/zycore/TypeTraits.hpp"
    "include/zycore/Utils.hpp")
set(sources
    "src/BinaryStream.cpp"
    "src/Property.cpp"
    "src/ReflectableObject.cpp"
    "src/SignalObject.cpp")

if (ZYCORE_HEADER_ONLY)
    add_library("Zycore" INTERFACE)
    target_include_directories("Zycore" INTERFACE "include/")
else ()
    add_library("Zycore" ${headers} ${sources})
    include_directories("include/")

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
            "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(initial_compile_flags "-std=c++14")
        if (ZYCORE_DEV)
            set(initial_compile_flags "${initial_compile_flags} -Werror")
        endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(initial_compile_flags "/W4 /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS /GR-")
        if (ZYCORE_DEV)
            set(initial_compile_flags "${initial_compile_flags} /WX")
        endif ()
    endif ()

    set(ZYCORE_COMPILE_FLAGS "${initial_compile_flags}"
        CACHE STRING "Flags used when compiling the ZyCore library.")
    mark_as_advanced(ZYCORE_COMPILE_FLAGS)
    set_target_properties("Zycore" PROPERTIES COMPILE_FLAGS "${ZYCORE_COMPILE_FLAGS}")
endif ()
