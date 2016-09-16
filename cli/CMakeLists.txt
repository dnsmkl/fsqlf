project(fsqlf_cli)
# Command line fsqlf and example config file.


add_executable(fsqlf
    cli.c
    main.c
    ../utils/string/read_int.c)


include_directories(${CMAKE_SOURCE_DIR}/include)
# target_include_directories() needs cmake 2.8.12.


target_link_libraries(fsqlf libfsqlf_static)


# get_property(fsqlf_location TARGET fsqlf PROPERTY LOCATION)
add_custom_target(formatting.conf ALL
    COMMAND $<TARGET_FILE:fsqlf> --create-config-file
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    DEPENDS fsqlf)


set_target_properties(fsqlf PROPERTIES
    SKIP_BUILD_RPATH FALSE
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN"
    INSTALL_RPATH_USE_LINK_PATH TRUE)


install(TARGETS fsqlf DESTINATION bin)
install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/formatting.conf
    DESTINATION share/fsqlf
    RENAME formatting.conf.example)
