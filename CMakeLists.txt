cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    bfcxx
    VERSION 0.1.0
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(prevent-in-source-builds)
include(CPM)

add_library(
    bfcxx_lib OBJECT
    source/bfcxx/lexer.cpp
)

target_include_directories(
    bfcxx_lib
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/bfcxx>"
)

target_compile_features(bfcxx_lib PUBLIC cxx_std_23)

add_executable(bfcxx_exe source/main.cpp)
add_executable(bfcxx::exe ALIAS bfcxx_exe)

set_target_properties(bfcxx_exe PROPERTIES
    OUTPUT_NAME bfcxx
)
target_compile_features(bfcxx_exe PRIVATE cxx_std_23)
target_link_libraries(bfcxx_exe PRIVATE bfcxx_lib)

# Testing
if(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

if(BUILD_TESTING)
    message(STATUS "Building tests")
    add_subdirectory(test)
endif()
