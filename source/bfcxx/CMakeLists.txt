include(GenerateExportHeader)

add_library(bfcxx_lib parser.cpp interpret.cpp)
add_library(bfcxx::lib ALIAS bfcxx_lib)

target_include_directories(
  bfcxx_lib PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                   $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)

target_compile_features(bfcxx_lib PUBLIC cxx_std_20)

set_target_properties(
  bfcxx_lib
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES
)

if(BFCXX_WITH_WRAPAROUND)
  target_compile_definitions(bfcxx_lib PUBLIC BFCXX_WITH_WRAPAROUND)
endif()

generate_export_header(
  bfcxx_lib EXPORT_FILE_NAME
  ${PROJECT_BINARY_DIR}/include/bfcxx/bfcxx_lib_export.hpp
)
