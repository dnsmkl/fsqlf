# This script is needed, to cross-compile from linux to windows.
# (compile on linux computer, but produce windows executables)


# Name of the target operating system.
SET(CMAKE_SYSTEM_NAME Windows)


# Compilers for C and C++.
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)


# Here is the target environment located.
SET(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32)


# Adjust where FIND_XXX() commands search:
# - headers and libraries in target environment.
# - programs in host environment.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# references:
#   http://www.vtk.org/Wiki/CMake_Cross_Compiling


# cmake 2.8.0 has bug regarding cross-compiling.
#   file: /usr/share/cmake-2.8/Modules/FindwxWidgets.cmake
# It was fixed on 2014-08-22:
#   https://cmake.org/gitweb?p=cmake.git;a=commitdiff;h=33286235048495ceafb636d549d9a4e8891967ae
#   -if(WIN32 AND NOT CYGWIN AND NOT MSYS)
#   +if(WIN32 AND NOT CYGWIN AND NOT MSYS AND NOT CMAKE_CROSSCOMPILING)
#      set(wxWidgets_FIND_STYLE "win32")
#    else()
#   -  if(UNIX OR MSYS)
#   -    set(wxWidgets_FIND_STYLE "unix")
#   -  endif()
#   +  set(wxWidgets_FIND_STYLE "unix")
#    endif()
# Also see: http://opencpn.org/ocpn/node/306
