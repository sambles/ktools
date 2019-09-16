cmake_minimum_required(VERSION 3.14)
project(ktools)

# Create the VERSION define
set(KTOOLS_VERSION "3.1.0")
configure_file(config.h.cmake_in config.h)

ENABLE_TESTING()

# Turn on C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)
find_package(Boost 1.56 REQUIRED COMPONENTS log unit_test_framework)

# Make BOOST auto-generate main method for unit tests
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

# Turn on all warnings, for all compiler flavours
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    MESSAGE( STATUS "using clang settings" )
    add_compile_options( -Wall -Wextra -Wunused-parameter)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    MESSAGE( STATUS "using gnu settings" )
    add_compile_options( -Wall -Wextra -Wunused-parameter)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    MESSAGE( STATUS "using msvc settings" )
    add_compile_options( /wd4503 ) # truncated symbol
    add_compile_options( /wd4702 ) # unreachable code
    add_compile_options( /wd4091 ) # typedef ignored on left when no variable is declared
    add_compile_options( /bigobj )
    add_definitions( /DUNICODE /D_UNICODE ) # it is a new millenium
endif()

add_test(filetool ./src/filetool/filetool_tests)
add_test(fmcalc ./src/fmcalc/fmcalc_tests)
add_test(ktest ./ktest/runtests.sh)
add_test(ktest-extended ./ktest/runtestsx.sh)

add_subdirectory(src)
