cmake_minimum_required (VERSION 2.6)
project (Cinderella)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

# Build LLVM in current source folder;
set (LLVM_DIR "${PROJECT_SOURCE_DIR}/llvm/build/lib/cmake/llvm")

# Looking for dependencies;
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Project source code directory: ${PROJECT_SOURCE_DIR}")

# The version number;
set (CINDERELLA_VERSION_MAJOR 0)
set (CINDERELLA_VERSION_MINOR 1)
set (CINDERELLA_AUTHOR YHSPY)
# Set c++ version;
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS_RELEASE "-O3")
set (OUTPUT "Cinderella")

# Configure a header file to pass some of the CMake settings to the source code;

# configure_file (
#         "${PROJECT_SOURCE_DIR}/Cinderella.h.in"
#         "${PROJECT_SOURCE_DIR}/Cinderella.h"
# )

# Add the binary tree to the search path for include files, so that we will find headers;
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/llvm/include")
include_directories("${PROJECT_SOURCE_DIR}/llvm/build/include")

add_subdirectory (support/)
aux_source_directory (. DIR_SRCS)

# Add the executable;
add_executable(${OUTPUT} ${DIR_SRCS})

# Find the libraries that correspond to the LLVM components that we wish to use (all);
llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD})

# Link against LLVM libraries
target_link_libraries (${OUTPUT} ${llvm_libs} support)

install(TARGETS ${OUTPUT}
        RUNTIME DESTINATION bin)
