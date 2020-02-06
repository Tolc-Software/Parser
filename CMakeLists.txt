cmake_minimum_required(VERSION 3.15)

project(Parser VERSION 0.1 LANGUAGES CXX)

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use ccache if available
find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Add custom scripts
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY git@github.com:srydell/IntermediateRepresentation.git
  GIT_TAG master)

FetchContent_MakeAvailable(IRSpecification)

include(LLVMUtilities)
# Download if necessary and get the clang libs
# Defines llvm_entry_{SOURCE_DIR,INCLUDE_DIR} variables
setup_llvm(NAME "LLVM_ENTRY")

# Macro to get downloaded Clang and llvm packages
get_clang_and_llvm(PATH ${llvm_entry_SOURCE_DIR})
# From the LLVM cmake module. Defines add_llvm_{executable,library}
include(AddLLVM)
include(AddClang)

# Define project_warnings and project_options targets
include(WarningsAndOptions)

add_llvm_library(Parser
                 src/Consumer/ParserConsumer.cpp
                 src/Frontend/ParserFrontendAction.cpp
                 src/Builders/commonBuilder.cpp
                 src/Builders/functionBuilder.cpp
                 src/Builders/namespaceBuilder.cpp
                 src/Builders/structBuilder.cpp
                 src/Helpers/walkIRStructure.cpp
                 src/Helpers/Utils/split.cpp
                 src/Parser/Parse.cpp
                 src/Visitor/ParserVisitor.cpp
                 src/Visitor/VisitCXXRecordDecl.cpp
                 src/Visitor/VisitFunctionDecl.cpp
                 src/Visitor/VisitNamespaceDecl.cpp)

target_link_libraries(Parser
                      PUBLIC  IR::IR
                      PRIVATE clangASTMatchers
                              clangBasic
                              clangTooling
                              project_options
                              project_warnings)

# Ignore warnings from clang/ as if they were system headers.
# NOTE: This is only available on clang
target_include_directories(Parser SYSTEM PRIVATE ${llvm_entry_INCLUDE_DIR})
target_include_directories(Parser PRIVATE src)
target_include_directories(Parser PUBLIC include)
set_target_properties(Parser
                      PROPERTIES CXX_STANDARD_REQUIRED
                                 ON
                                 CXX_EXTENSIONS
                                 OFF
                                 CXX_STANDARD
                                 17)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()
endif()
