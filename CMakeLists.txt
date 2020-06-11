cmake_minimum_required(VERSION 3.16)

project(
  Parser
  VERSION 0.1
  LANGUAGES CXX)

# Add custom scripts
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(Doxygen)
enable_doxygen()

# Set ccache, ipo, etc.
include(cmake/ProjectSettings.cmake)

# Define add_{warnings,options}
include(cmake/WarningsAndOptions.cmake)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

# Define enable_sanitizers
include(cmake/Sanitizers.cmake)

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY git@github.com:srydell/IntermediateRepresentation.git
  GIT_TAG master)

FetchContent_MakeAvailable(IRSpecification)

# Define run_conan
include(cmake/ConanHelpers.cmake)
run_conan(REQUIRES
  CTRE/v2.8.2@ctre/stable)

include(cmake/LLVMUtilities.cmake)
# Download if necessary and get the clang libs Defines
# llvm_entry_{SOURCE_DIR,INCLUDE_DIR} variables
set(llvm_entry_version 10.0.0)
setup_llvm(NAME "LLVM_ENTRY" VERSION ${llvm_entry_version})

# Macro to get downloaded Clang and llvm packages
get_clang_and_llvm(PATH ${llvm_entry_SOURCE_DIR})
# From the LLVM cmake module. Defines add_llvm_{executable,library}
include(AddLLVM)

# Defines get_platform_specific_includes and format_includes
include(cmake/IncludePathHelpers.cmake)

# Set the include path for the system library in the variable
# We are using the standard library shipped
# with the downloaded llvm for include paths in the parsing
get_system_include(
  VARIABLE
    platform_include
  LLVM_DIRECTORY
    ${llvm_entry_SOURCE_DIR}
  LLVM_VERSION
    ${llvm_entry_version})

format_includes(VARIABLE INCLUDE_PATH INCLUDES
                ${platform_include} SYSTEM)
configure_file(src/Helpers/includes.cpp.in
               ${CMAKE_CURRENT_LIST_DIR}/src/Helpers/includes.cpp @ONLY)

add_llvm_library(
  Parser
  src/Builders/commonBuilder.cpp
  src/Builders/enumBuilder.cpp
  src/Builders/functionBuilder.cpp
  src/Builders/namespaceBuilder.cpp
  src/Builders/structBuilder.cpp
  src/Builders/typeBuilder.cpp
  src/Consumer/ParserConsumer.cpp
  src/Frontend/ParserFrontendAction.cpp
  src/Helpers/Type/Matchers/container.cpp
  src/Helpers/Type/matchers.cpp
  src/Helpers/Type/utilities.cpp
  src/Helpers/Utils/combine.cpp
  src/Helpers/Utils/split.cpp
  src/Helpers/Utils/string.cpp
  src/Helpers/includes.cpp
  src/Helpers/walkIRStructure.cpp
  src/Parser/Parse.cpp
  src/Visitor/ParserVisitor.cpp
  src/Visitor/VisitCXXRecordDecl.cpp
  src/Visitor/VisitEnumDecl.cpp
  src/Visitor/VisitFieldDecl.cpp
  src/Visitor/VisitFunctionDecl.cpp
  src/Visitor/VisitNamespaceDecl.cpp)

target_link_libraries(
  Parser
  PUBLIC IR::IR
  PRIVATE clangASTMatchers clangBasic clangTooling
  CONAN_PKG::CTRE)

add_warnings(TARGET Parser)
add_options(TARGET Parser)

# Set sanitizer options on Parser if options defined
enable_sanitizers(Parser)

# Ignore warnings from clang/ as if they were system headers.
target_include_directories(Parser SYSTEM PRIVATE ${llvm_entry_INCLUDE_DIR})
target_include_directories(Parser PRIVATE src)
target_include_directories(Parser PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:include>)

set_target_properties(
  Parser PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)

# Export the library to link against
add_library(Tolc::Parser ALIAS Parser)

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

  option(ENABLE_PACKAGING "Build the packages" OFF)
  if(ENABLE_PACKAGING)
    include(GNUInstallDirs)
    install(TARGETS Parser EXPORT ParserTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(EXPORT ParserTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Parser NAMESPACE Tolc::)
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/Parser
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(TARGETS IR EXPORT IRTargets PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/IR)
    install(EXPORT IRTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IR NAMESPACE IR::)
  endif()
endif()
