cmake_minimum_required(VERSION 3.14)

project(
    cppcrc
    VERSION 1.3.0
    LANGUAGES CXX
    DESCRIPTION "A very small, fast, header-only, C++ library for generating CRCs"
)

add_library(cppcrc INTERFACE)

target_include_directories(cppcrc INTERFACE include/
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# To include cppcrc in your project, just add the following two lines to your CMakeLists.txt file
#   add_subdirectory(cppcrc/)
#   target_link_libraries(your_project_name cppcrc)
