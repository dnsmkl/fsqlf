project(fsqlf_gui)
# GUI for fsqlf.


# Ordering is important for mingw: "adv" must go before "core".
find_package(wxWidgets REQUIRED adv core base aui)
include(${wxWidgets_USE_FILE})
set(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)


# Remove -mthreads, so there would be no need for mingwm10.dll.
list(REMOVE_ITEM wxWidgets_LIBRARIES -mthreads)
string(REGEX REPLACE "-mthreads" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


add_executable(wx_fsqlf
    basic_notepad.cpp
    dnd_target.cpp
    wx_fsqlf.cpp
    license_text.h)


include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
# target_include_directories() needs cmake 2.8.12.


target_link_libraries(wx_fsqlf ${wxWidgets_LIBRARIES} libfsqlf_static)


# "About" window: version string from git.
set(git_cmd "git")
set(git_arg1 "describe")
set(git_arg2 "master")
execute_process(COMMAND ${git_cmd} ${git_arg1} ${git_arg2}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE git_describe1)
string(STRIP ${git_describe1} git_describe2)
message(STATUS "git describe master: ${git_describe2} (not portable)")
add_definitions(-DVERSION="${git_describe2}")


# "About" window: license text.
add_executable(text_to_header EXCLUDE_FROM_ALL
    ${CMAKE_SOURCE_DIR}/utils/text_to_header/text_to_header.c)
add_custom_command(OUTPUT license_text.h
    COMMAND
        $<TARGET_FILE:text_to_header>
        ${CMAKE_SOURCE_DIR}/LICENSE license_text.h
        LICENSE_TEXT
    DEPENDS text_to_header
    COMMENT "Generating: license_text.h")


install(TARGETS wx_fsqlf DESTINATION bin)
