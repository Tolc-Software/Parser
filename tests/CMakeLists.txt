include_guard()

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY git@github.com:catchorg/Catch2.git
  GIT_TAG v2.12.1)

FetchContent_MakeAvailable(Catch2)

# Everything put in TestUtil/include is available
add_library(TestUtil INTERFACE)
target_include_directories(TestUtil INTERFACE TestUtil/include)

# These are needed to use create_catch2_test
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

# Defines create_catch2_test
include(${modules}/Catch2Helpers.cmake)

if(ENABLE_COVERAGE)
  include(${modules}/CodeCoverage.cmake)

  # Use coverage of parser
  target_code_coverage(Parser)

  add_code_coverage_all_targets()
endif()

foreach(
  test

  Helpers/Type/Matchers/container.cpp
  Helpers/Type/matchers.cpp
  Helpers/Utils/split.cpp
  Helpers/Utils/string.cpp
  Helpers/commandLineArgs.cpp
  Parser/Parse.cpp
  Parser/Windows/systemIncludeHelper.cpp
  arrays.cpp
  autos.cpp
  classes.cpp
  complex.cpp
  constructorsDestructors.cpp
  dataStructs.cpp
  deques.cpp
  enums.cpp
  fields.cpp
  files.cpp
  forward_lists.cpp
  functionTypes.cpp
  functions.cpp
  globalVariables.cpp
  lists.cpp
  maps.cpp
  multimaps.cpp
  multipleClasses.cpp
  multisets.cpp
  namespaces.cpp
  optional.cpp
  paths.cpp
  priority_queues.cpp
  queues.cpp
  references.cpp
  representation.cpp
  sets.cpp
  smartPointers.cpp
  stacks.cpp
  statics.cpp
  strings.cpp
  templates.cpp
  tuples.cpp
  unordered_maps.cpp
  unordered_multimaps.cpp
  unordered_multisets.cpp
  unordered_sets.cpp
  userDefined.cpp
  valarrays.cpp
  variant.cpp
  vectors.cpp

  )

  # Ex: PybindProxy/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${test} NAME_WLE)

  create_catch2_test(
    TARGET
    ${testName}
    SOURCES
    ${test}
    INCLUDE
    ${PROJECT_SOURCE_DIR}/src
    LINK_LIBRARIES
    clangTooling
    clangBasic
    clangASTMatchers
    Parser
    fmt::fmt
    TestUtil)

  target_include_directories(${testName} SYSTEM
                             PRIVATE ${llvm_entry_INCLUDE_DIR})

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})
endforeach()

