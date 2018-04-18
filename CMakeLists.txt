cmake_minimum_required (VERSION 2.6)
project (Cinderella)

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
# Set c++ version;
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS_RELEASE "-O3")


# Configure a header file to pass some of the CMake settings to the source code;

# configure_file (
#         "${PROJECT_SOURCE_DIR}/Global.in"
#         "${PROJECT_SOURCE_DIR}/Lexer.h"
# )

# Add the binary tree to the search path for include files, so that we will find headers;
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/llvm/include")
include_directories("${PROJECT_SOURCE_DIR}/llvm/build/include")


# Add the executable;
add_executable(Cinderella Cinderella.cc Cinderella.h Lexer.cc Lexer.h BinaryExprAST.cc BinaryExprAST.h CallExprAST.cc CallExprAST.h
        ExprAST.cc ExprAST.h FunctionAST.cc FunctionAST.h LLVMBinder.cc LLVMBinder.h NumberExprAST.cc NumberExprAST.h Parser.h Parser.cc
        PrototypeAST.cc PrototypeAST.h VariableExprAST.cc VariableExprAST.h)

# Find the libraries that correspond to the LLVM components that we wish to use;
llvm_map_components_to_libnames(llvm_libs core native mcjit support irreader)

# Link against LLVM libraries
target_link_libraries(Cinderella ${llvm_libs})
