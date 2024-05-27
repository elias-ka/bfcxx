cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BFCXX_WITH_WRAPAROUND "Enable wraparound for the tape" ON)
option(BFCXX_BUILD_TESTS "Build tests" OFF)

project(
  bfcxx
  VERSION 0.1.0
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(prevent-in-source-builds)
include(CPM)

add_subdirectory(source)

# Testing
if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

if(BFCXX_BUILD_TESTS)
  message(STATUS "Building tests")
  add_subdirectory(test)
endif()
