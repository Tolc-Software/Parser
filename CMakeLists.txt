cmake_minimum_required(VERSION 3.16)

project(
  Parser
  VERSION 0.4.0
  LANGUAGES CXX)

configure_file(docs/ReleaseNotes/version.in
               ${CMAKE_CURRENT_LIST_DIR}/docs/ReleaseNotes/version @ONLY)

include(FetchContent)
FetchContent_Declare(
  cmake_modules
  GIT_REPOSITORY https://github.com/Tolc-Software/cmake-modules.git
  GIT_TAG main)
FetchContent_MakeAvailable(cmake_modules)
# Basis of all shared CMake modules
set(modules ${cmake_modules_SOURCE_DIR}/cmake)

include(${modules}/Doxygen.cmake)
enable_doxygen()

# Set ccache, ipo, etc.
include(${modules}/ProjectSettings.cmake)

# Define add_{warnings,options}
include(${modules}/WarningsAndOptions.cmake)

# allow for static analysis options
include(${modules}/StaticAnalyzers.cmake)

# Define enable_sanitizers
include(${modules}/Sanitizers.cmake)

FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY https://github.com/Tolc-Software/IntermediateRepresentation.git
  GIT_TAG v0.13.0)
FetchContent_MakeAvailable(IRSpecification)

FetchContent_Declare(
  ctre_content
  GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
  GIT_TAG v3.4.1)
FetchContent_Populate(ctre_content)

# Define run_conan
include(${modules}/ConanHelpers.cmake)
# Create the profile from variables known during configure
setup_conan_profile(VARIABLE parser_conan_profile)
# This helps with packaging
set(CONAN_REQUIRES_PACKAGES spdlog/1.9.2 fmt/8.1.1)
find_conan_packages(
  REQUIRES ${CONAN_REQUIRES_PACKAGES} PROFILE
  ${parser_conan_profile})

include(${modules}/LLVMUtilities.cmake)
# Download if necessary and get the clang libs Defines
# llvm_entry_{SOURCE_DIR,INCLUDE_DIR} variables
set(llvm_entry_version 12.0.0)
setup_llvm(NAME "LLVM_ENTRY" VERSION ${llvm_entry_version})

# Macro to get downloaded Clang and llvm packages
get_clang_and_llvm(PATH ${llvm_entry_SOURCE_DIR})
# From the LLVM cmake module. Defines add_llvm_{executable,library}
include(AddLLVM)

# Defines get_system_include and format_includes
include(${modules}/IncludePathHelpers.cmake)

# Set the include path for the system library in the variable
# We are using the standard library shipped
# with the downloaded llvm for include paths in the parsing
get_system_include(VARIABLE platform_include LLVM_DIRECTORY
                   ${llvm_entry_SOURCE_DIR} LLVM_VERSION ${llvm_entry_version})

format_includes(VARIABLE INCLUDE_PATH INCLUDES ${platform_include} SYSTEM)

configure_file(src/Helpers/commandLineArgs.cpp.in
               ${CMAKE_CURRENT_LIST_DIR}/src/Helpers/commandLineArgs.cpp @ONLY)

add_llvm_library(
  Parser
  src/Builders/commonBuilder.cpp
  src/Builders/enumBuilder.cpp
  src/Builders/fieldBuilder.cpp
  src/Builders/functionBuilder.cpp
  src/Builders/namespaceBuilder.cpp
  src/Builders/structBuilder.cpp
  src/Builders/typeBuilder.cpp
  src/Consumer/ParserConsumer.cpp
  src/Frontend/ParserFrontendAction.cpp
  src/Helpers/Type/Matchers/base.cpp
  src/Helpers/Type/Matchers/container.cpp
  src/Helpers/Type/Matchers/function.cpp
  src/Helpers/Type/matchers.cpp
  src/Helpers/Type/utilities.cpp
  src/Helpers/Utils/combine.cpp
  src/Helpers/Utils/split.cpp
  src/Helpers/Utils/string.cpp
  src/Helpers/commandLineArgs.cpp
  src/Helpers/getStructData.cpp
  src/Helpers/walkIRStructure.cpp
  src/Parser/Parse.cpp
  src/Parser/Windows/systemIncludeHelper.cpp
  src/Template/cache.cpp
  src/Visitor/Helpers/getDocumentation.cpp
  src/Visitor/Helpers/parseFunction.cpp
  src/Visitor/ParserVisitor.cpp
  src/Visitor/VisitCXXRecordDecl.cpp
  src/Visitor/VisitClassTemplateSpecializationDecl.cpp
  src/Visitor/VisitEnumDecl.cpp
  src/Visitor/VisitFieldDecl.cpp
  src/Visitor/VisitFunctionDecl.cpp
  src/Visitor/VisitFunctionTemplateDecl.cpp
  src/Visitor/VisitNamespaceDecl.cpp
  src/Visitor/VisitVarDecl.cpp)

target_link_libraries(
  Parser
  PUBLIC IR::IR
  PRIVATE clangASTMatchers clangBasic clangTooling spdlog::spdlog fmt::fmt)

add_warnings(TARGET Parser)
add_options(TARGET Parser)

# Set sanitizer options on Parser if options defined
enable_sanitizers(Parser)

# Ignore warnings from {clang,ctre} as if they were system headers.
target_include_directories(Parser SYSTEM PRIVATE ${llvm_entry_INCLUDE_DIR})
target_include_directories(Parser SYSTEM PRIVATE ${ctre_content_SOURCE_DIR}/single-header)
target_include_directories(Parser PRIVATE src)
target_include_directories(
  Parser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                $<INSTALL_INTERFACE:include>)

set_target_properties(Parser PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS
                                                                 OFF)

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

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" OFF)
  if(ENABLE_PACKAGING)
    add_subdirectory(tools/packaging)
    if(ENABLE_TESTING)
      add_subdirectory(tests/packaging)
    endif()
  endif()
endif()
