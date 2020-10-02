cmake_minimum_required (VERSION 2.6)
project (Cinderella)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)
SET(PROJECT_SOURCE_DIR ./src)
SET(LLVM_BUILT_DIR $ENV{LLVM_BUILT_DIR})

# Looking for dependencies;
find_package(LLVM REQUIRED CONFIG)

# Set version number;
set (CINDERELLA_VERSION_MAJOR 0)
set (CINDERELLA_VERSION_MINOR 1)
set (CINDERELLA_AUTHOR YHSPY)

# Set c++ version;
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set (CMAKE_CXX_FLAGS_RELEASE "-O3")
set (OUTPUT "cinderella")

# Add the binary tree to the search path for include files, so that we will find headers;
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${LLVM_BUILT_DIR}/include")
include_directories("${LLVM_BUILT_DIR}/build/include")

add_subdirectory (./support/)
aux_source_directory (. DIR_SRCS)

# Add the executable;
add_executable(${OUTPUT} ${DIR_SRCS})

# Find the libraries that correspond to the LLVM components that we wish to use (all);
llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD})

# Link against LLVM libraries;
target_link_libraries (${OUTPUT} ${llvm_libs} support)

