project(test_fsqlf_cli)
# Test fsqlf cli.


add_executable(format_files_test format_files_test.c tools/file_compare.c)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cases)
# get_property(PATH_FSQLF_CLI TARGET fsqlf PROPERTY LOCATION)

add_definitions(
    -DPATH_TC_STATICSQL="${CMAKE_CURRENT_SOURCE_DIR}/cases/"
    -DPATH_TC_GENERATED="${CMAKE_CURRENT_BINARY_DIR}/cases/"
    -DPATH_FSQLF_CLI="$<TARGET_FILE:fsqlf>")
# target_compile_definitions() requires newer version of cmake.
