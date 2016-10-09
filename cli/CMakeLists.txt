project(fsqlf_cli)
# Sql formatter on command line.


set(fsqlf_sources
    cli.c
    main.c
    ../utils/string/read_int.c)


add_executable(fsqlf ${fsqlf_sources})
set_target_properties(fsqlf PROPERTIES C_STANDARD 99)
target_include_directories(fsqlf PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fsqlf libfsqlf_static)


add_custom_target(formatting.conf ALL
    COMMAND $<TARGET_FILE:fsqlf> --create-config-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/formatting.conf
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    DEPENDS fsqlf)


install(TARGETS fsqlf DESTINATION bin)
install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/formatting.conf
    DESTINATION share/fsqlf
    RENAME formatting.conf.example)
