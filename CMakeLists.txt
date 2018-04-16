cmake_minimum_required (VERSION 2.6)
project (Cinderella)

# The version number.
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "${PROJECT_SOURCE_DIR}/Global.in"
        "${PROJECT_BINARY_DIR}/Lexer.h"
)

# add the binary tree to the search path fr include files
# so that we will find headers
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(Cinderella Lexer.cc)