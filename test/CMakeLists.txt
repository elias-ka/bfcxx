if (NOT TARGET Catch2::Catch2WithMain)
    CPMAddPackage("gh:catchorg/Catch2@3.6.0")
endif ()

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

enable_testing()
include(CTest)
include(Catch)

add_executable(bfcxx_tests
        test_parser.cpp
)
target_compile_features(bfcxx_tests PRIVATE cxx_std_20)
target_link_libraries(bfcxx_tests PRIVATE Catch2::Catch2WithMain bfcxx_lib)

catch_discover_tests(bfcxx_tests)
