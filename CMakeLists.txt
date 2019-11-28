cmake_minimum_required(VERSION 3.15)

project(
  Parser
  VERSION 0.1
  LANGUAGES CXX)

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use ccache if available
find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# If no build type is provided explicitly, set it to Release
if(NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "" FORCE)
  endif()
endif()

# Add custom scripts
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY git@github.com:srydell/IntermediateRepresentation.git
  GIT_TAG master)

FetchContent_MakeAvailable(IRSpecification)

# Download if necessary and get the clang libs
# Defines llvm_include_dir variable
include(LLVMUtilities)
setupLLVM()

# Define project_warnings and project_options targets
include(WarningsAndOptions)

add_executable(Parser src/Parser.cpp)
target_link_libraries(Parser PRIVATE project_warnings project_options)
target_link_libraries(Parser PRIVATE IR::IR)
target_link_libraries(Parser PRIVATE clangTooling clangBasic clangASTMatchers)
target_include_directories(Parser PRIVATE ${llvm_include_dir})
set_target_properties(Parser PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS
                                        OFF CXX_STANDARD 17)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    # enable_testing()
    # add_subdirectory(tests)
    # TODO: Add benchmarks
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()
endif()
