project(fsqlf_lib)
# Lib of fsqlf.


find_package(FLEX)
FLEX_TARGET(lexyy formatter/fsqlf.lex ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c
       COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lex.yy.h)



set(libfsqlf_sources
    ${FLEX_lexyy_OUTPUTS}
    ../utils/queue/queue.c
    ../utils/stack/stack.c
    ../utils/string/read_int.c
    conf_file/conf_file_create.c
    conf_file/conf_file_read.c
    formatter/lex_wrapper.c
    formatter/print_keywords.c
    formatter/tokque.c
    kw/kw.c
    kw/kwmap.c
    lex/token.c)


add_library(libfsqlf SHARED ${libfsqlf_sources})
set_target_properties(libfsqlf PROPERTIES C_STANDARD 99)
set_target_properties(libfsqlf PROPERTIES LIBRARY_OUTPUT_NAME fsqlf)


add_library(libfsqlf_static STATIC ${libfsqlf_sources})
set_target_properties(libfsqlf_static PROPERTIES LIBRARY_OUTPUT_NAME fsqlf_static)


include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/formatter
    ${CMAKE_SOURCE_DIR}/include)
# target_include_directories() needs cmake 2.8.12.


install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
install(TARGETS libfsqlf
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
