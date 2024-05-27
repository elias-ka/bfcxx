add_executable(bfcxx_cli main.cpp)
add_executable(bfcxx::cli ALIAS bfcxx_cli)

set_target_properties(bfcxx_cli PROPERTIES OUTPUT_NAME bfcxx)
target_compile_features(bfcxx_cli PRIVATE cxx_std_20)
target_link_libraries(bfcxx_cli PRIVATE bfcxx_lib)
