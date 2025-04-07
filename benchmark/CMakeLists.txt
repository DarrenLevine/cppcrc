cmake_minimum_required(VERSION 3.10)

project(benchmark
    VERSION 1.0
    DESCRIPTION "Benchmarking several C++ CRC libraries"
    LANGUAGES CXX)

add_executable(benchmark main.cpp)
target_compile_options(benchmark PUBLIC -Wall -std=c++17 -O3)

# for https://github.com/DarrenLevine/cppcrc
target_include_directories(benchmark PRIVATE ..)

# for https://github.com/pstolarz/CRaC/tree/master
include(FetchContent)
FetchContent_Populate(crac
    GIT_REPOSITORY https://github.com/pstolarz/CRaC.git
    GIT_TAG "origin/master")
add_library(crac INTERFACE)
target_include_directories(crac SYSTEM INTERFACE "${crac_SOURCE_DIR}/include")
target_link_libraries(benchmark PRIVATE crac)

# for https://github.com/d-bahr/CRCpp
include(FetchContent)
FetchContent_Populate(crccpp
    GIT_REPOSITORY https://github.com/d-bahr/CRCpp.git
    GIT_TAG "origin/master")
add_library(crccpp INTERFACE)
target_include_directories(crccpp SYSTEM INTERFACE "${crccpp_SOURCE_DIR}/inc")
add_subdirectory("${crccpp_SOURCE_DIR}")
target_link_libraries(benchmark PRIVATE crccpp)

# for boost crc
find_package(Boost 1.72.0)
IF (Boost_FOUND)
    ADD_DEFINITIONS("-DHAS_BOOST")
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(benchmark ${Boost_LIBRARIES})
ENDIF()

# for https://github.com/martinus/nanobench
FetchContent_Declare(
    nanobench
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG v4.1.0
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(nanobench)
target_link_libraries(benchmark PRIVATE nanobench)
